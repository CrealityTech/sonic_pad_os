#include <QtDebug>
#include <QEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets/QMainWindow>
#include <QInputDialog>
#include <QWindow>
#include <QKeyEvent>
#include <QDebug>
#include <QScreen>
#include <QThread>

#include "inputcontext.h"
#include "QPinyinInputMethod.h"
#include "QKeyboardLayout.h"

#define WINDOW_HEIGHT QApplication::desktop()->height()

InputContext::InputContext() :
	QPlatformInputContext(),
        m_inputPanel(nullptr),
	m_focusObject(nullptr),
	m_filterEvent(nullptr),
	m_visible(false),
	m_needHiden(true)
{
	inputMethod = new QPinyinInputMethod(this);
	qobject_cast<QPinyinInputMethod*>(inputMethod)->setInputContext(this);
	inputMethodHints = Qt::ImhNone;

	QKeyboardLayout().LoadLayout(":/layout/main.json");
}

InputContext::~InputContext()
{
	if (m_inputPanel) {
		delete m_inputPanel;
		m_inputPanel = nullptr;
	}
}

bool InputContext::isValid() const
{
	return true;
}

QRectF InputContext::keyboardRect() const
{
	return m_inputPanel ? m_inputPanel->rect() : QRect();
}

bool InputContext::isAnimating() const
{
    return m_inputPanel ? m_inputPanel->isAnimating() : false;
}

void InputContext::commit(const QString &text, int replaceFrom, int replaceLength)
{
	QList<QInputMethodEvent::Attribute> attributes;
	//attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::Selection,0,0,QVariant()));

	QInputMethodEvent inputEvent(QString(), attributes);
	inputEvent.setCommitString(text, replaceFrom, replaceLength);

	sendEvent(&inputEvent);
}

void InputContext::commit()
{

}

void InputContext::showInputPanel()
{
	if (!m_visible) {
		m_visible = true;
	}
	updateInputPanelVisible();
}

void InputContext::hideInputPanel()
{
	if (m_visible && m_needHiden)
		m_visible = false;

	updateInputPanelVisible();

	inputMethod->reset();
}

bool InputContext::isInputPanelVisible() const
{
	return m_inputPanel ? m_inputPanel->isVisible() : false;
}

QObject *InputContext::focusObject()
{
	return m_focusObject;
}

void InputContext::setFocusObject(QObject *object)
{
	if (m_focusObject != object)
	{
		if (m_focusObject)
		{   
			if (!preeditText.isEmpty())
				setPreeditText(QString());

			m_focusObject->removeEventFilter(this);
		}

		m_focusObject = object;

		if (m_focusObject)
			m_focusObject->installEventFilter(this);

		//emit focusObjectChanged();
	}
        m_visible = true;
        update(Qt::ImEnabled);
}


void InputContext::update(Qt::InputMethodQueries queries)
{
    Q_UNUSED(queries);

	bool enabled = inputMethodQuery(Qt::ImEnabled).toBool();

	if (enabled && !m_inputPanel) {
		m_inputPanel = new KeyboardContainer();
		m_inputPanel->setObjectName("Qt5KeyBoard");
		connect(m_inputPanel, &KeyboardContainer::hideKeyboard, this, &InputContext::hideKeyboard);
		connect(m_inputPanel, &KeyboardContainer::keyPressed, inputMethod, &QAbstractInputMethod::keyEvent);
		connect(m_inputPanel, &KeyboardContainer::changeLanguage, inputMethod, &QAbstractInputMethod::changeLanguage);
		connect(m_inputPanel, &KeyboardContainer::chooseCandidate, inputMethod, &QAbstractInputMethod::chooseCandidate);

		connect(inputMethod, &QAbstractInputMethod::showCandidateList, m_inputPanel, &KeyboardContainer::setCandidateList);
		connect(inputMethod, &QAbstractInputMethod::showLanguageName, m_inputPanel, &KeyboardContainer::setLanguageName);

		connect(m_inputPanel, &QObject::destroyed, [this]() { m_inputPanel = nullptr; });

		m_inputPanel->resize(600, 300);
		m_inputPanel->move((QApplication::desktop()->width() - m_inputPanel->width()) / 2, WINDOW_HEIGHT - m_inputPanel->height());
	}

	//if (m_inputContext) {
	if (enabled)
	{
		//m_inputContext->priv()->update(queries);
		if (m_visible)
		{
			m_needHiden = false;
			updateInputPanelVisible();
		}
	}
	else
	{
		m_needHiden = true;
		hideInputPanel();
	}
	// m_inputContext->priv()->setFocus(enabled);
	//}

	Qt::InputMethodHints inputMethodHints = Qt::InputMethodHints(inputMethodQuery(Qt::ImHints).toInt());
	bool newInputMethodHints = inputMethodHints != this->inputMethodHints;
	this->inputMethodHints = inputMethodHints;

	if (newInputMethodHints && m_inputPanel)
		m_inputPanel->setInputMethodHints(inputMethodHints);
}

void InputContext::sendKeyEvent(QKeyEvent *event)
{
	const QGuiApplication *app = qApp;
	QWindow *focusWindow = app ? app->focusWindow() : nullptr;
	if (focusWindow) {
		m_filterEvent = event;
		QGuiApplication::sendEvent(focusWindow, event);
		m_filterEvent = nullptr;
	}
}

void InputContext::sendEvent(QEvent *event)
{
	if (m_focusObject) {
		m_filterEvent = event;
		QGuiApplication::sendEvent(m_focusObject, event);
		m_filterEvent = nullptr;
	}
}

void InputContext::hideKeyboard()
{
	if (m_inputPanel)
	{
		m_inputPanel->hide();
		m_inputPanel->onHideSymbol();
	}
}


bool testAttribute(const QList<QInputMethodEvent::Attribute> &attributes, QInputMethodEvent::AttributeType attributeType)
{
	for (const QInputMethodEvent::Attribute &attribute : qAsConst(attributes)) {
		if (attribute.type == attributeType)
			return true;
	}
	return false;
}

void InputContext::sendPreedit(const QString &text, const QList<QInputMethodEvent::Attribute> &attributes, int replaceFrom, int replaceLength)
{
	preeditText = text;

	QInputMethodEvent event(text, attributes);
	const bool replace = replaceFrom != 0 || replaceLength > 0;
	if (replace)
		event.setCommitString(QString(), replaceFrom, replaceLength);

	sendEvent(&event);

	// Send also to shadow input if only attributes changed.
	// In this case the update() may not be called, so the shadow
	// input may be out of sync.
	//    if (_shadow.inputItem() && !replace && !text.isEmpty() &&
	//            !textChanged && attributesChanged) {
	//        VIRTUALKEYBOARD_DEBUG() << "QVirtualKeyboardInputContextPrivate::sendPreedit(shadow)"
	//#ifdef SENSITIVE_DEBUG
	//               << text << replaceFrom << replaceLength
	//#endif
	//            ;
	//        event.setAccepted(true);
	//        QGuiApplication::sendEvent(_shadow.inputItem(), &event);
	//}
}

void InputContext::updateInputPanelVisible()
{
	if (!m_inputPanel)
		return;

	if (m_visible != m_inputPanel->isVisible()) {
		if (m_visible)
            m_inputPanel->animationShow();
		else
            m_inputPanel->animationHide();

		//emit InputPanelVisibleChanged();
	}
}

QVariant InputContext::inputMethodQuery(Qt::InputMethodQuery query)
{
	QInputMethodQueryEvent event(query);
	sendEvent(&event);
	return event.value(query);
}


void InputContext::setPreeditText(const QString &text, QList<QInputMethodEvent::Attribute> attributes, int replaceFrom, int replaceLength)
{
	// Add default attributes
	if (!text.isEmpty()) {
		if (!testAttribute(attributes, QInputMethodEvent::TextFormat)) {
			QTextCharFormat textFormat;
			textFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
			attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 0, text.length(), textFormat));
		}
	} /*else if (d->_forceCursorPosition != -1) {
		d->addSelectionAttribute(attributes);
	}*/

	sendPreedit(text, attributes, replaceFrom, replaceLength);
}

void InputContext::sendKeyClick(int key, const QString &text, int modifiers)
{
	QKeyEvent pressEvent(QEvent::KeyPress, key, Qt::KeyboardModifiers(modifiers), text);
	QKeyEvent releaseEvent(QEvent::KeyRelease, key, Qt::KeyboardModifiers(modifiers), text);

	sendKeyEvent(&pressEvent);
	sendKeyEvent(&releaseEvent);
}

