#include "QPinyinInputMethod.h"
#include "pinyindecoderservice_p.h"
#include <QPointer>
#include <QVector>
#include <QDebug>
#include "inputcontext.h"

class QPinyinInputMethodPrivate
{
    Q_DECLARE_PUBLIC(QPinyinInputMethod)

public:
    enum State
    {
        Idle,
        Input,
        Predict
    };

    QPinyinInputMethodPrivate(QPinyinInputMethod *q_ptr) :
        q_ptr(q_ptr),
        inputMode(QPinyinInputMethod::InputMode::Latin),
        pinyinDecoderService(PinyinDecoderService::getInstance()),
        state(Idle),
        surface(),
        totalChoicesNum(0),
        candidatesList(),
        fixedLen(0),
        composingStr(),
        activeCmpsLen(0),
        finishSelection(true),
        posDelSpl(-1),
        isPosInSpl(false)
    {
        pinyinDecoderService->setUserDictionary(true);
    }

    void resetToIdleState()
    {
        Q_Q(QPinyinInputMethod);

        InputContext *inputContext = q->inputContext();

        //        // Disable the user dictionary when entering sensitive data
        //        if (inputContext) {
        //            bool userDictionaryEnabled = !inputContext->inputMethodHints().testFlag(Qt::ImhSensitiveData);
        //            if (userDictionaryEnabled != pinyinDecoderService->isUserDictionaryEnabled())
        //                pinyinDecoderService->setUserDictionary(userDictionaryEnabled);
        //        }

        if (state == Idle)
            return;

        state = Idle;
        surface.clear();
        fixedLen = 0;
        finishSelection = true;
        composingStr.clear();
        if (inputContext)
            inputContext->setPreeditText(QString());
        activeCmpsLen = 0;
        posDelSpl = -1;
        isPosInSpl = false;

        resetCandidates();
    }

    bool addSpellingChar(QChar ch, bool reset)
    {
        if (reset) {
            surface.clear();
            pinyinDecoderService->resetSearch();
        }
        if (ch == Qt::Key_Apostrophe) {
            if (surface.isEmpty())
                return false;
            if (surface.endsWith(ch))
                return true;
        }
        surface.append(ch);
        return true;
    }

    bool removeSpellingChar()
    {
        if (surface.isEmpty())
            return false;
        QVector<int> splStart = pinyinDecoderService->spellingStartPositions();
        isPosInSpl = (surface.length() <= splStart[fixedLen + 1]);
        posDelSpl = isPosInSpl ? fixedLen - 1 : surface.length() - 1;
        return true;
    }

    void chooseAndUpdate(int candId)
    {
        Q_Q(QPinyinInputMethod);

        //联想状态选择联想词
        if (state == Predict)
            choosePredictChoice(candId);
        else//对输入按键解码
            chooseDecodingCandidate(candId);

        if (composingStr.length() > 0)
        {
            if ((candId >= 0 || finishSelection) && composingStr.length() == fixedLen)
            {
                QString resultStr = getComposingStrActivePart();
                tryPredict();
                q->inputContext()->commit(resultStr);
            }
            else if (state == Idle)
            {
                state = Input;
            }
        }
        else
        {
            tryPredict();
        }
    }

    bool chooseAndFinish()
    {
        if (state == Predict || !totalChoicesNum)
            return false;
        chooseAndUpdate(0);
        if (state != Predict && totalChoicesNum > 0)
            chooseAndUpdate(0);
        return true;
    }

    int candidatesCount()
    {
        return totalChoicesNum;
    }

    QString candidateAt(int index)
    {
        if (index < 0 || index >= totalChoicesNum)
            return QString();
        if (index >= candidatesList.size()) {
            int fetchMore = qMin(index + 20, totalChoicesNum - candidatesList.size());
            candidatesList.append(pinyinDecoderService->fetchCandidates(candidatesList.size(), fetchMore, fixedLen));
            if (index == 0 && totalChoicesNum == 1) {
                int surfaceDecodedLen = pinyinDecoderService->pinyinStringLength(true);
                if (surfaceDecodedLen < surface.length())
                    candidatesList[0] = candidatesList[0] + surface.mid(surfaceDecodedLen).toLower();
            }
        }
        return index < candidatesList.size() ? candidatesList[index] : QString();
    }

    void chooseDecodingCandidate(int candId)
    {
        Q_Q(QPinyinInputMethod);
        Q_ASSERT(state != Predict);


        int result = 0;
        if (candId < 0) {
            if (surface.length() > 0) {
                if (posDelSpl < 0) {
                    result = pinyinDecoderService->search(surface);
                } else {
                    result = pinyinDecoderService->deleteSearch(posDelSpl, isPosInSpl, false);
                    posDelSpl = -1;
                }
            }
        } else {
            if (totalChoicesNum > 1) {
                result = pinyinDecoderService->chooceCandidate(candId);
            } else {
                QString resultStr;
                if (totalChoicesNum == 1) {
                    QString undecodedStr = candId < candidatesList.length() ? candidatesList.at(candId) : QString();
                    resultStr = pinyinDecoderService->candidateAt(0).mid(0, fixedLen) + undecodedStr;
                }
                resetToIdleState();
                if (!resultStr.isEmpty())
                    q->inputContext()->commit(resultStr);
                return;
            }
        }

        resetCandidates();
        totalChoicesNum = result;

        surface = pinyinDecoderService->pinyinString(false);
        QVector<int> splStart = pinyinDecoderService->spellingStartPositions();
        QString fullSent = pinyinDecoderService->candidateAt(0);
        fixedLen = pinyinDecoderService->fixedLength();
        composingStr = fullSent.mid(0, fixedLen) + surface.mid(splStart[fixedLen + 1]);
        activeCmpsLen = composingStr.length();

        // Prepare the display string.
        QString composingStrDisplay;
        int surfaceDecodedLen = pinyinDecoderService->pinyinStringLength(true);
        if (!surfaceDecodedLen) {
            composingStrDisplay = composingStr.toLower();
            if (!totalChoicesNum)
                totalChoicesNum = 1;
        } else {
            activeCmpsLen = activeCmpsLen - (surface.length() - surfaceDecodedLen);
            composingStrDisplay = fullSent.mid(0, fixedLen);
            for (int pos = fixedLen + 1; pos < splStart.size() - 1; pos++) {
                composingStrDisplay += surface.mid(splStart[pos], splStart[pos + 1] - splStart[pos]).toUpper();
                if (splStart[pos + 1] < surfaceDecodedLen)
                    composingStrDisplay += QLatin1String("'");
            }
            if (surfaceDecodedLen < surface.length())
                composingStrDisplay += surface.mid(surfaceDecodedLen).toLower();
        }

        q->inputContext()->setPreeditText(composingStrDisplay);

        finishSelection = splStart.size() == (fixedLen + 2);
        if (!finishSelection)
            candidateAt(0);
    }

    void choosePredictChoice(int choiceId)
    {
        Q_ASSERT(state == Predict);

        if (choiceId < 0 || choiceId >= totalChoicesNum)
            return;

        QString tmp = candidatesList.at(choiceId);

        resetCandidates();

        candidatesList.append(tmp);
        totalChoicesNum = 1;

        surface.clear();
        fixedLen = tmp.length();
        composingStr = tmp;
        activeCmpsLen = fixedLen;

        finishSelection = true;
    }

    QString getComposingStrActivePart()
    {
        return composingStr.mid(0, activeCmpsLen);
    }

    void resetCandidates()
    {
//        Q_Q(QPinyinInputMethod);

        candidatesList.clear();
        if (totalChoicesNum) {
            totalChoicesNum = 0;
        }
    }

    void updateCandidateList()
    {
        Q_Q(QPinyinInputMethod);
        emit q->showCandidateList(candidatesList);
        //        emit q->selectionListChanged(QVirtualKeyboardSelectionListModel::Type::WordCandidateList);
        //        emit q->selectionListActiveItemChanged(QVirtualKeyboardSelectionListModel::Type::WordCandidateList,
        //                                               totalChoicesNum > 0 && state == PinyinInputMethodPrivate::Input ? 0 : -1);
    }

    bool canDoPrediction()
    {
//        Q_Q(QPinyinInputMethod);
        //        QVirtualKeyboardInputContext *inputContext = q->inputContext();
        //        return inputMode == QVirtualKeyboardInputEngine::InputMode::Pinyin &&
        //                composingStr.length() == fixedLen &&
        //                inputContext &&
        //                !inputContext->inputMethodHints().testFlag(Qt::ImhNoPredictiveText);

        return false;
    }

    void tryPredict()
    {
        // Try to get the prediction list.
        if (canDoPrediction()) {
//            Q_Q(QPinyinInputMethod);
            if (state != Predict)
                resetToIdleState();
            //QVirtualKeyboardInputContext *inputContext = q->inputContext();
            //int cursorPosition = inputContext->cursorPosition();
            //int historyStart = qMax(0, cursorPosition - 3);
            //QString history = inputContext->surroundingText().mid(historyStart, cursorPosition - historyStart);
            //candidatesList = pinyinDecoderService->predictionList(history);
            totalChoicesNum = candidatesList.size();
            finishSelection = false;
            state = Predict;
        } else {
            resetCandidates();
        }

        if (!candidatesCount())
            resetToIdleState();
    }

    QPinyinInputMethod *q_ptr;
    QPinyinInputMethod::InputMode inputMode;
    QPointer<PinyinDecoderService> pinyinDecoderService;
    State state;
    QString surface;
    int totalChoicesNum;
    QList<QString> candidatesList;
    int fixedLen;
    QString composingStr;
    int activeCmpsLen;
    bool finishSelection;
    int posDelSpl;
    bool isPosInSpl;
};

class ScopedCandidateListUpdate
{
    Q_DISABLE_COPY(ScopedCandidateListUpdate)
public:
    inline explicit ScopedCandidateListUpdate(QPinyinInputMethodPrivate *d) :
        d(d),
        candidatesList(d->candidatesList),
        totalChoicesNum(d->totalChoicesNum),
        state(d->state)
    {
    }

    inline ~ScopedCandidateListUpdate()
    {
        if (totalChoicesNum != d->totalChoicesNum || state != d->state || candidatesList != d->candidatesList)
        {
            QList<QString> candidateList;
            for(int i = 0;i < d->totalChoicesNum;++i)
                candidateList.append(d->candidateAt(i));

            d->updateCandidateList();
        }
    }

private:
    QPinyinInputMethodPrivate *d;
    QList<QString> candidatesList;
    int totalChoicesNum;
    QPinyinInputMethodPrivate::State state;
};


QPinyinInputMethod::QPinyinInputMethod(QObject *parent) :
	QAbstractInputMethod(parent),
	d_ptr(new QPinyinInputMethodPrivate(this)),
	m_inputContext(nullptr)
{
}

QPinyinInputMethod::~QPinyinInputMethod()
{
    delete d_ptr;
}

InputContext *QPinyinInputMethod::inputContext() const
{
    return m_inputContext;
}

void QPinyinInputMethod::setInputContext(InputContext * context)
{
	m_inputContext = context;
}



bool QPinyinInputMethod::keyEvent(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers)
    Q_D(QPinyinInputMethod);
    if (d->inputMode == QPinyinInputMethod::InputMode::Pinyin)
    {
        ScopedCandidateListUpdate scopedCandidateListUpdate(d);
        Q_UNUSED(scopedCandidateListUpdate)

        if ((key >= Qt::Key_A && key <= Qt::Key_Z) || (key == Qt::Key_Apostrophe))
        {
            if (d->state == QPinyinInputMethodPrivate::Predict)
                d->resetToIdleState();
            if (d->addSpellingChar(text.at(0), d->state == QPinyinInputMethodPrivate::Idle))
            {
                d->chooseAndUpdate(-1);
                return true;
            }
        }
        else if (key == Qt::Key_Space)
        {
            if (d->state != QPinyinInputMethodPrivate::Predict && d->candidatesCount() > 0)
            {
                d->chooseAndUpdate(0);
                return true;
            }
        }
        else if (key == Qt::Key_Return)
        {
            if (d->state != QPinyinInputMethodPrivate::Predict && d->candidatesCount() > 0)
            {
                QString surface = d->surface;
                d->resetToIdleState();
                inputContext()->commit(surface);
                return true;
            }
        }
        else if (key == Qt::Key_Backspace)
        {
            if(d->state == d->Idle)
                inputContext()->sendKeyClick(key,text,modifiers);
            else if (d->removeSpellingChar())
            {
                d->chooseAndUpdate(-1);
                return true;
            }
        }
        else
        {
            if(d->state == d->Input)
            {
                if(text.isEmpty())
                    d->chooseAndFinish();
            }
            else if(d->state == d->Idle)
            {
                if(key == Qt::Key_Enter){
                    inputContext()->hideKeyboard();
                }
                inputContext()->sendKeyClick(key,text,modifiers);
            }
        }
    }
    else
    {
        if(key == Qt::Key_Enter){
            inputContext()->hideKeyboard();
        }
        inputContext()->sendKeyClick(key,text,modifiers);
    }
    return false;

}

void QPinyinInputMethod::reset()
{
    Q_D(QPinyinInputMethod);
    ScopedCandidateListUpdate scopedCandidateListUpdate(d);
    Q_UNUSED(scopedCandidateListUpdate)
    d->resetToIdleState();
}

void QPinyinInputMethod::changeLanguage()
{
    Q_D(QPinyinInputMethod);
    if(d->inputMode == QAbstractInputMethod::InputMode::Pinyin)
    {
        d->inputMode = QAbstractInputMethod::InputMode::Latin;
        showLanguageName("English");
    }
    else
    {
        d->inputMode = QAbstractInputMethod::InputMode::Pinyin;
        showLanguageName("简体中文");
    }
    ScopedCandidateListUpdate scopedCandidateListUpdate(d);
    Q_UNUSED(scopedCandidateListUpdate)
    d->resetToIdleState();
    inputContext()->setPreeditText("");
}

void QPinyinInputMethod::chooseCandidate(int id)
{
    Q_D(QPinyinInputMethod);
    d->chooseAndUpdate(id);
    d->updateCandidateList();
}
