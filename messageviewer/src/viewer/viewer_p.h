/*
  Copyright (c) 1997 Markus Wuebben <markus.wuebben@kde.org>
  Copyright (C) 2009 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.net
  Copyright (c) 2009 Andras Mantia <andras@kdab.net>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef MAILVIEWER_P_H
#define MAILVIEWER_P_H

#include <MimeTreeParser/NodeHelper>
#include "config-messageviewer.h"
#include "viewer.h" //not so nice, it is actually for the enums from MailViewer
#include "PimCommon/ShareServiceUrlManager"
#include "messageviewer/viewerplugininterface.h"
#include <WebEngineViewer/CheckPhishingUrlJob>

#include <AkonadiCore/item.h>
#include <AkonadiCore/monitor.h>
#include <AkonadiCore/session.h>
#include <kio/job.h>
#include <KMime/Message>
#include <kservice.h>
#include <ksharedconfig.h>
#include <QPointer>
#include <QUrl>

#include <QObject>
#include <QTimer>

namespace KIO {
class Job;
}

class QAction;
class KActionCollection;
class KSelectAction;
class KToggleAction;
class QMenu;
class KActionMenu;

class QPoint;
class QSplitter;
class QModelIndex;
class QPrinter;

namespace KPIMTextEdit {
class SlideContainer;
class TextToSpeechWidget;
}
namespace PimCommon {
class ShareServiceUrlManager;
}

namespace MimeTreeParser {
class AttachmentStrategy;
class HtmlWriter;
class ObjectTreeParser;
}

namespace WebEngineViewer {
class WebHitTestResult;
class FindBarWebEngineView;
class ZoomActionMenu;
class LocalDataBaseManager;
}
namespace MessageViewer {
class HeaderStylePlugin;
class CSSHelper;
class MailWebEngineView;
class WebEnginePartHtmlWriter;
class HtmlStatusBar;
class ScamDetectionWarningWidget;
class MimePartTreeView;
class OpenAttachmentFolderWidget;
class HeaderStyleMenuManager;
class ViewerPluginToolManager;
class ViewerPluginInterface;
class SubmittedFormWarningWidget;
class MailSourceWebEngineViewer;
/**
\brief Private class for the Viewer, the main widget in the messageviewer library.

This class creates all subwidgets, like the MailWebView, the HtmlStatusBar and the FindBarMailWebView.

Also, ViewerPrivate creates and exposes all actions.

\par Displaying a message

Before displaying a message, a message needs to be set. This can be done in two ways, with
setMessageItem() and with setMessage(). setMessageItem() is the preferred way, as the viewer can
then remember the Akonadi::Item belonging to the message. The Akonadi::Item is needed when modifying
the message, for example when editing or deleting an attachment.
Sometimes passing an Akonadi::Item to the viewer is not possible, for example when double-clicking
an attached message, in which case a new KMime::Message is constructed out of the attachment, and a
separate window is opened for it. In this case, the KMime::Message has no associated Akonadi::Item.
If there is an Akonadi::Item available, it will be monitored for changes and the viewer
automatically updated on external changes.

Once a message is set, update() is called. update() can also be called after the message has already
been displayed. As an example, this is the case when the user decides to decrypt the message. The
decryption can happen async, and once the decryption is finished, update() is called to display the
now decrypted content. See the documentation of MimeTreeParser::ObjectTreeParser on how exactly decryption is
handled.
update() is just a thin wrapper that calls updateReaderWin(). The only difference is that update()
has a timer that prevents too many slow calls to updateReaderWin() in a short time frame.
updateReaderWin() again is only a thin wrapper that resets some state and then calls
displayMessage().
displayMessage() itself is again a thin wrapper, which starts the MimeTreeParser::HtmlWriter and then calls
parseMsg().
Finally, parseMsg() does the real work. It uses MimeTreeParser::ObjectTreeParser ::parseObjectTree() to let the
MimeTreeParser::ObjectTreeParser parse the message and generate the HTML code for it.
As mentioned before, it can happen that the MimeTreeParser::ObjectTreeParser needs to do some operation that happens
async, for example decrypting. In this case, the MimeTreeParser::ObjectTreeParser will create a BodyPartMemento,
which basically is a wrapper around the job that does the async operation. Once the async operation
is finished. the BodyPartMemento will trigger an update() of ViewerPrivate, so that
MimeTreeParser::ObjectTreeParser ::parseObjectTree() gets called again and the MimeTreeParser::ObjectTreeParser then can generate
HTML which has the decrypted content of the message. Again, see the documentation of MimeTreeParser::ObjectTreeParser for the details.
Additionally, parseMsg() does some evil hack for saving unencrypted messages should the config
option for that be set.

\par Displaying a MIME part of the message

The viewer can show only a part of the message, for example by clicking on a MIME part in the
message structure viewer or by double-clicking an attached message. In this case, setMessagePart()
is called. There are two of these functions. One even has special handling for images, special
handling for binary attachments and special handling of attached messages. In the last case, a new
KMime::Message is constructed and set as the main message with setMessage().

\par Attachment Handling

Some of those actions are actions that operate on a single attachment. For those, there is usually
a slot, like slotAttachmentCopy(). These actions are triggered from the attachment context menu,
which is shown in showAttachmentPopup(). The actions are connected to slotHandleAttachment() when
they are activated.
The action to edit an attachment uses the EditorWatcher to detect when editing with an external
editor is finished. Upon finishing, slotAttachmentEditDone() is called, which then creates an
ItemModifyJob to store the changes of the attachment. A map of currently active EditorWatcher and
their KMime::Content is available in mEditorWatchers.
For most attachment actions, the attachment is first written to a temp file. The action is then
executed on this temp file. Writing the attachment to a temp file is done with
MimeTreeParser::NodeHelper::writeNodeToTempFile(). This method is called before opening or copying an attachment or
when rendering the attachment list. The MimeTreeParser::ObjectTreeParser also calls MimeTreeParser::NodeHelper::writeNodeToTempFile()
in some places. Once the temp file is written, MimeTreeParser::NodeHelper::tempFileUrlFromNode() can be used to get
the file name of the temp file for a specific MIME part. This is for example used by the handler for
'attachment:' URLs, AttachmentURLHandler.

Since URLs for attachments are in the "attachment:" scheme, dragging them as-is to outside applications
wouldn't work, since other applications don't understand this scheme. Therefore, the viewer has
special handling for dragging URLs: In eventFilter(), drags are detected, and the URL handler is
called to deal with the drag. The attachment URL handler then starts a drag with the file:// URL
of the temp file of the attachment, which it gets with MimeTreeParser::NodeHelper::tempFileUrlFromNode().

TODO: How are attachment handled that are loaded on demand? How does prepareHandleAttachment() work?
TODO: This temp file handling is a big mess and could use a rewrite, especially in the face of load
      on demand. There shouldn't be the need to write out tempfiles until really needed.

Some header styles display an attachment list in the header. The HTML code for the attachment list
cannot be generated by the HeaderStyle itself, since that does not know about all attachments.
Therefore, the attachment list needs to be created by ViewerPrivate. For this, the HeaderStyle
writes out a placeholder for the attachment list when it creates the HTML for the header. Once the
MimeTreeParser::ObjectTreeParser is finished with the message, injectAttachments() is called. injectAttachments()
searches for the placeholder and replaces that with the real HTML code for the attachments.

One of the attachment actions is to scoll to the attachment. That action is only available when
right-clicking the header. The action scrolls to the attachment in the body and draws a yellow frame
around the attachment. This is done in scrollToAttachment(). The attachment in the body and the div
which is used for the colored frame are both created by the MimeTreeParser::ObjectTreeParser .

\par Misc

ViewerPrivate holds the MimeTreeParser::NodeHelper, which is passed on to the MimeTreeParser::ObjectTreeParser when it needs it.
It also holds the HeaderStyle, HeaderStrategy, MimeTreeParser::AttachmentStrategy, CSSHelper, MimeTreeParser::HtmlWriter and more,
some of them again passed to the MimeTreeParser::ObjectTreeParser when it needs it.

@author andras@kdab.net
 */
class ViewerPrivate : public QObject
{
    Q_OBJECT
public:

    ViewerPrivate(Viewer *aParent, QWidget *mainWindow, KActionCollection *actionCollection);

    virtual ~ViewerPrivate();

    /** Returns message part from given URL or null if invalid. The URL's path is a KMime::ContentIndex path, or an index for the extra nodes,
    followed by : and the ContentIndex path. */
    KMime::Content *nodeFromUrl(const QUrl &url) const;

    /** Open the attachment pointed to the node.
    * @param fileName - if not empty, use this file to load the attachment content
    */
    void openAttachment(KMime::Content *node, const QUrl &url);

    /** Delete the attachment the @param node points to. Returns false if the user
    cancelled the deletion, true in all other cases (including failure to delete
    the attachment!) */
    bool deleteAttachment(KMime::Content *node, bool showWarning = true);

    void attachmentProperties(KMime::Content *node);
    void attachmentCopy(const KMime::Content::List &contents);

    /** Edit the attachment the @param node points to. Returns false if the user
    cancelled the editing, true in all other cases! */
    void editAttachment(KMime::Content *node, bool showWarning = true);

    void scrollToAnchor(const QString &anchor);

    void showAttachmentPopup(KMime::Content *node, const QString &name, const QPoint &p);

    /**
    * Sets the current attachment ID and the current attachment temporary filename
    * to the given values.
    * Call this so that slotHandleAttachment() knows which attachment to handle.
    */
    void prepareHandleAttachment(KMime::Content *node);

    void postProcessMessage(MimeTreeParser::ObjectTreeParser *otp, MimeTreeParser::KMMsgEncryptionState encryptionState);

    QString createAtmFileLink(const QString &atmFileName) const;
    KService::Ptr getServiceOffer(KMime::Content *content);
    KMime::Content::List selectedContents();
    void attachmentOpenWith(KMime::Content *node, const KService::Ptr &offer = KService::Ptr());
    void attachmentOpen(KMime::Content *node);

    /** Return the MimeTreeParser::HtmlWriter connected to the MailWebView we use */
    MimeTreeParser::HtmlWriter *htmlWriter() const;

    HeaderStylePlugin *headerStylePlugin() const;

    CSSHelper *cssHelper() const;

    MimeTreeParser::NodeHelper *nodeHelper() const;

    Viewer *viewer() const;

    Akonadi::Item messageItem() const;

    KMime::Message::Ptr message() const;

    /** Returns whether the message should be decryted. */
    bool decryptMessage() const;

    /** Display a generic HTML splash page instead of a message. */
    void displaySplashPage(const QString &templateName, const QVariantHash &data, const QByteArray &domain = QByteArray());

    void displaySplashPage(const QString &message);

    /** Enable the displaying of messages again after an splash (or other) page was displayed */
    void enableMessageDisplay();

    /** Feeds the HTML viewer with the contents of the given message.
    HTML begin/end parts are written around the message. */
    void displayMessage();

    /** Parse the given content and generate HTML out of it for display */
    void parseContent(KMime::Content *content);

    /** Creates a nice mail header depending on the current selected
    header style. */
    QString writeMsgHeader(KMime::Message *aMsg, KMime::Content *vCardNode = nullptr, bool topLevel = false);

    /** show window containing information about a vCard. */
    void showVCard(KMime::Content *msgPart);

    void saveMainFrameScreenshotInFile(const QString &filename);

private:
    /** HTML initialization. */
    void initHtmlWidget();
    void createOpenWithMenu(QMenu *topMenu, const QString &contentTypeStr, bool fromCurrentContent);
public:
    /** Read settings from app's config file. */
    void readConfig();

    /** Write settings to app's config file. Calls sync() if withSync is true. */
    void writeConfig(bool withSync = true);

    /** Get/set the message attachment strategy. */
    const MimeTreeParser::AttachmentStrategy *attachmentStrategy() const;
    void setAttachmentStrategy(const MimeTreeParser::AttachmentStrategy *strategy);

    /** Get selected override character encoding.
      @return The encoding selected by the user or an empty string if auto-detection
      is selected. */
    QString overrideEncoding() const;

    /** Set the override character encoding. */
    void setOverrideEncoding(const QString &encoding);

    /** Set printing mode */
    void setPrinting(bool enable);
    bool printingMode() const;

    /** Print message. */
    void printMessage(const Akonadi::Item &msg);
    void printPreviewMessage(const Akonadi::Item &message);

    void resetStateForNewMessage();

    void setMessageInternal(const KMime::Message::Ptr &message, MimeTreeParser::UpdateMode updateMode);

    /** Set the Akonadi item that will be displayed.
    *  @param item - the Akonadi item to be displayed. If it doesn't hold a mail (KMime::Message::Ptr as payload data),
    *                an empty page is shown.
    *  @param updateMode - update the display immediately or not. See MailViewer::UpdateMode.
    */
    void setMessageItem(const Akonadi::Item &item, MimeTreeParser::UpdateMode updateMode = MimeTreeParser::Delayed);

    /** Set the message that shall be shown.
    * @param msg - the message to be shown. If 0, an empty page is displayed.
    * @param updateMode - update the display immediately or not. See MailViewer::UpdateMode.
    */
    void setMessage(const KMime::Message::Ptr &msg, MimeTreeParser::UpdateMode updateMode = MimeTreeParser::Delayed);

    /** Instead of settings a message to be shown sets a message part
      to be shown */
    void setMessagePart(KMime::Content *node);

    /** Show or hide the Mime Tree Viewer if configuration
      is set to smart mode.  */
    void showHideMimeTree();

    /** View message part of type message/RFC822 in extra viewer window. */
    void atmViewMsg(const KMime::Message::Ptr &message);

    void adjustLayout();
    void createWidgets();
    void createActions();

    void showContextMenu(KMime::Content *content, const QPoint &point);

    KToggleAction *actionForAttachmentStrategy(const MimeTreeParser::AttachmentStrategy *);
    /** Read override codec from configuration */
    void readGlobalOverrideCodec();

    /** Get codec corresponding to the currently selected override character encoding.
      @return The override codec or 0 if auto-detection is selected. */
    const QTextCodec *overrideCodec() const;

    QString renderAttachments(KMime::Content *node, const QColor &bgColor) const;

    KMime::Content *findContentByType(KMime::Content *content, const QByteArray &type); //TODO(Andras) move to MimeTreeParser::NodeHelper

    /** Return a QTextCodec for the specified charset.
    * This function is a bit more tolerant, than QTextCodec::codecForName */
    static const QTextCodec *codecForName(const QByteArray &_str); //TODO(Andras) move to a utility class?

    /** Saves the relative position of the scroll view. Call this before calling update()
      if you want to preserve the current view. */
    void saveRelativePosition();

    bool htmlMail() const;
    bool htmlLoadExternal() const;

    bool htmlMailGlobalSetting() const;

    /** Get the html override setting */
    Viewer::DisplayFormatMessage displayFormatMessageOverwrite() const;

    /** Override default html mail setting */
    void setDisplayFormatMessageOverwrite(Viewer::DisplayFormatMessage format);

    /** Get the load external references override setting */
    bool htmlLoadExtOverride() const;

    /** Override default load external references setting */
    void setHtmlLoadExtOverride(bool override);

    /** Enforce message decryption. */
    void setDecryptMessageOverwrite(bool overwrite = true);

    /** Show signature details. */
    bool showSignatureDetails() const;

    /** Show signature details. */
    void setShowSignatureDetails(bool showDetails = true);

    /* show or hide the list that points to the attachments */
    void setShowAttachmentQuicklist(bool showAttachmentQuicklist = true);

    /* show or hide encryption details */
    void setHideEncryptionDetails(bool encDetails = true);

    void scrollToAttachment(KMime::Content *node);
    void setUseFixedFont(bool useFixedFont);

    void attachmentView(KMime::Content *atmNode);
    void setFullToAddressList(bool showFullTo);
    void setFullCcAddressList(bool showFullCc);

    /** Show/Hide the field with id "field" */
    void toggleFullAddressList(const QString &field);

    void setZoomFactor(qreal zoomFactor);

    void goOnline();
    void goResourceOnline();

    void showOpenAttachmentFolderWidget(const QUrl &url);

    bool mimePartTreeIsEmpty() const;

    void setPluginName(const QString &pluginName);

    QList<QAction *> viewerPluginActionList(
        MessageViewer::ViewerPluginInterface::SpecificFeatureTypes features);
    QList<QAction *> interceptorUrlActions(const WebEngineViewer::WebHitTestResult &result) const;

    void setPrintElementBackground(bool printElementBackground);
    bool showEmoticons() const;
    void checkPhishingUrl();
    void executeRunner(const QUrl &url);
    QUrl imageUrl() const;

private Q_SLOTS:
    void slotActivatePlugin(MessageViewer::ViewerPluginInterface *interface);
    void slotModifyItemDone(KJob *job);
    void slotMessageMayBeAScam();
    void slotMessageIsNotAScam();
    void slotAddToWhiteList();
    void slotFormSubmittedForbidden();

    /** Show hide all fields specified inside this function */
    void toggleFullAddressList();

    void itemFetchResult(KJob *job);

    void slotItemChanged(const Akonadi::Item &item, const QSet<QByteArray> &partIdentifiers);
    void slotItemMoved(const Akonadi::Item &, const Akonadi::Collection &, const Akonadi::Collection &);

    void itemModifiedResult(KJob *job);

    void collectionFetchedForStoringDecryptedMessage(KJob *job);

    void slotClear();

    void slotMessageRendered();

    void slotOpenWithAction(QAction *act);

    void slotOpenWithActionCurrentContent(QAction *act);

    void slotOpenWithDialog();

    void slotOpenWithDialogCurrentContent();

    void saveSplitterSizes() const;

    void slotRefreshMessage(const Akonadi::Item &item);
    void slotServiceUrlSelected(PimCommon::ShareServiceUrlManager::ServiceType serviceType);
    void slotStyleChanged(MessageViewer::HeaderStylePlugin *plugin);
    void slotStyleUpdated();
    void slotWheelZoomChanged(int numSteps);
    void slotOpenInBrowser();
    void slotExportHtmlPageFailed();
    void slotExportHtmlPageSuccess(const QString &filename);
    void slotHandlePagePrinted(bool result);
    void slotToggleEmoticons();
public Q_SLOTS:
    /** An URL has been activate with a click. */
    void slotUrlOpen(const QUrl &url = QUrl());
    void slotOpenUrl();

    /** The mouse has moved on or off an URL. */
    void slotUrlOn(const QString &link);

    /** The user presses the right mouse button on an URL. */
    void slotUrlPopup(const WebEngineViewer::WebHitTestResult &result);

    /** The user selected "Find" from the menu. */
    void slotFind();

    /** The user toggled the "Fixed Font" flag from the view menu. */
    void slotToggleFixedFont();
    void slotToggleMimePartTree();

    /** Show the message source */
    void slotShowMessageSource();

    /** Refresh the reader window */
    void updateReaderWin();

    void slotMimePartSelected(const QModelIndex &index);

    void slotIconicAttachments();
    void slotSmartAttachments();
    void slotInlineAttachments();
    void slotHideAttachments();
    void slotHeaderOnlyAttachments();

    /** Some attachment operations. */
    void slotDelayedResize();

    /** Print message. Called on as a response of finished() signal of mPartHtmlWriter
      after rendering is finished.
      In the very end it deletes the KMReaderWin window that was created
      for the purpose of rendering. */
    void slotPrintMessage();
    void slotPrintPreview();

    void slotSetEncoding();
    void executeCustomScriptsAfterLoading();
    void slotSettingsChanged();
    void slotMimeTreeContextMenuRequested(const QPoint &pos);
    void slotAttachmentOpenWith();
    void slotAttachmentOpen();
    void slotAttachmentSaveAs();
    void slotAttachmentSaveAll();
    void slotAttachmentView();
    void slotAttachmentProperties();
    void slotAttachmentCopy();
    void slotAttachmentDelete();
    void slotAttachmentEdit();
    void slotLevelQuote(int l);

    /** Toggle display mode between HTML and plain text. */
    void slotToggleHtmlMode();
    void slotLoadExternalReference();

    /**
    * Does an action for the current attachment.
    * The action is defined by the KMHandleAttachmentCommand::AttachmentAction
    * enum.
    * prepareHandleAttachment() needs to be called before calling this to set the
    * correct attachment ID.
    */
    void slotHandleAttachment(int action);
    /** Copy the selected text to the clipboard */
    void slotCopySelectedText();

    void viewerSelectionChanged();

    /** Select message body. */
    void selectAll();

    /** Copy URL in mUrlCurrent to clipboard. Removes "mailto:" at
      beginning of URL before copying. */
    void slotUrlCopy();
    void slotSaveMessage();
    /** Re-parse the current message. */
    void update(MimeTreeParser::UpdateMode updateMode = MimeTreeParser::Delayed);

    void slotSpeakText();
    void slotCopyImageLocation();
    void slotSaveMessageDisplayFormat();
    void slotResetMessageDisplayFormat();

    void slotGeneralFontChanged();

Q_SIGNALS:
    void showStatusBarMessage(const QString &message);
    void popupMenu(const Akonadi::Item &msg, const QUrl &url, const QUrl &imageUrl, const QPoint &mousePos);
    void displayPopupMenu(const Akonadi::Item &msg, const WebEngineViewer::WebHitTestResult &result, const QPoint &mousePos);

    void urlClicked(const Akonadi::Item &msg, const QUrl &url);
    void requestConfigSync();
    void showReader(KMime::Content *aMsgPart, bool aHTML, const QString &encoding);
    void showMessage(const KMime::Message::Ptr &message, const QString &encoding);
    void replyMessageTo(const KMime::Message::Ptr &message, bool replyToAll);
    void itemRemoved();
    void makeResourceOnline(MessageViewer::Viewer::ResourceOnlineMode mode);

    void changeDisplayMail(Viewer::DisplayFormatMessage, bool);
    void moveMessageToTrash();
    void pageIsScrolledToBottom(bool);
    void printingFinished();
private:
    QString attachmentInjectionHtml();
    QString recipientsQuickListLinkHtml(const QString &);

    Akonadi::Relation relatedNoteRelation() const;
    void addHelpTextAction(QAction *act, const QString &text);
    void readGravatarConfig();

    void replyMessageToAuthor(KMime::Content *atmNode);
    void replyMessageToAll(KMime::Content *atmNode);
    bool urlIsAMalwareButContinue();

    void slotCheckedUrlFinished(const QUrl &url, WebEngineViewer::CheckPhishingUrlUtil::UrlStatus status);

    MimeTreeParser::NodeHelper *mNodeHelper = nullptr;
    void slotDelayPrintPreview();
public:
    bool mHtmlMailGlobalSetting;
    bool mHtmlLoadExternalGlobalSetting;
    bool mHtmlLoadExtOverride;
    KMime::Message::Ptr mMessage; //the current message, if it was set manually
    Akonadi::Item mMessageItem; //the message item from Akonadi
    // widgets:
    QSplitter *mSplitter = nullptr;
    QWidget *mBox = nullptr;
    HtmlStatusBar *mColorBar = nullptr;
#ifndef QT_NO_TREEVIEW
    MimePartTreeView *mMimePartTree = nullptr;
#endif
    MailWebEngineView *mViewer = nullptr;
    WebEngineViewer::FindBarWebEngineView *mFindBar = nullptr;

    const MimeTreeParser::AttachmentStrategy *mAttachmentStrategy = nullptr;
    QTimer mUpdateReaderWinTimer;
    QTimer mResizeTimer;
    QString mOverrideEncoding;
    QString mOldGlobalOverrideEncoding; // used to detect changes of the global override character encoding
    QString mPicsPath;

    /// This is true if the viewer currently is displaying a message. Can be false, for example when
    /// the splash/busy page is displayed.
    bool mMsgDisplay;

    CSSHelper *mCSSHelper = nullptr;
    bool mUseFixedFont;
    bool mPrinting;
    QWidget *mMainWindow = nullptr;
    KActionCollection *mActionCollection = nullptr;
    QAction *mCopyAction = nullptr;
    QAction *mCopyURLAction = nullptr;
    QAction *mUrlOpenAction = nullptr;
    QAction *mSelectAllAction = nullptr;
    QAction *mScrollUpAction = nullptr;
    QAction *mScrollDownAction = nullptr;
    QAction *mScrollUpMoreAction = nullptr;
    QAction *mScrollDownMoreAction = nullptr;
    QAction *mViewSourceAction = nullptr;
    QAction *mSaveMessageAction = nullptr;
    QAction *mFindInMessageAction = nullptr;
    QAction *mSaveMessageDisplayFormat = nullptr;
    QAction *mResetMessageDisplayFormat = nullptr;
    KToggleAction *mDisableEmoticonAction = nullptr;
    KToggleAction *mHeaderOnlyAttachmentsAction = nullptr;
    KSelectAction *mSelectEncodingAction = nullptr;
    KToggleAction *mToggleFixFontAction = nullptr;
    KToggleAction *mToggleDisplayModeAction = nullptr;
    KToggleAction *mToggleMimePartTreeAction = nullptr;
    QAction *mSpeakTextAction = nullptr;
    QAction *mCopyImageLocation = nullptr;
    QUrl mHoveredUrl;
    QUrl mClickedUrl;
    QUrl mImageUrl;
    QPoint mLastClickPosition;
    bool mCanStartDrag;
    MimeTreeParser::HtmlWriter *mHtmlWriter;
    /** Used only to be able to connect and disconnect finished() signal
      in printMsg() and slotPrintMsg() since mHtmlWriter points only to abstract non-QObject class. */
    QPointer<WebEnginePartHtmlWriter> mPartHtmlWriter;

    int mLevelQuote;
    bool mDecrytMessageOverwrite;
    bool mShowSignatureDetails;
    bool mShowAttachmentQuicklist;
    bool mForceEmoticons;
    int mRecursionCountForDisplayMessage;
    KMime::Content *mCurrentContent = nullptr;
    KMime::Content *mMessagePartNode = nullptr;
    QString mMessagePath;

    QColor mForegroundError;
    QColor mBackgroundError;

    Viewer *const q;
    Akonadi::Session *mSession = nullptr;
    Akonadi::Monitor mMonitor;
    QSet<AbstractMessageLoadedHandler *> mMessageLoadedHandlers;
    Akonadi::Item::Id mPreviouslyViewedItem;

    MessageViewer::ScamDetectionWarningWidget *mScamDetectionWarning = nullptr;
    MessageViewer::OpenAttachmentFolderWidget *mOpenAttachmentFolderWidget = nullptr;
    MessageViewer::SubmittedFormWarningWidget *mSubmittedFormWarning = nullptr;
    KPIMTextEdit::TextToSpeechWidget *mTextToSpeechWidget = nullptr;
    Viewer::DisplayFormatMessage mDisplayFormatMessageOverwrite;
    KPIMTextEdit::SlideContainer *mSliderContainer = nullptr;
    PimCommon::ShareServiceUrlManager *mShareServiceManager = nullptr;
    KActionMenu *mShareServiceUrlMenu = nullptr;
    MessageViewer::HeaderStylePlugin *mHeaderStylePlugin = nullptr;
    MessageViewer::HeaderStyleMenuManager *mHeaderStyleMenuManager = nullptr;
    MessageViewer::ViewerPluginToolManager *mViewerPluginToolManager = nullptr;
    WebEngineViewer::ZoomActionMenu *mZoomActionMenu = nullptr;
    QPrinter *mCurrentPrinter = nullptr;
    QList<QPointer<MessageViewer::MailSourceWebEngineViewer> > mListMailSourceViewer;
    WebEngineViewer::LocalDataBaseManager *mPhishingDatabase = nullptr;
};
}

#endif
