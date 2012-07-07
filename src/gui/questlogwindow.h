

#ifndef QUESTLOGWINDOW_H_
#define QUESTLOGWINDOW_H_

#include <vector>

#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/window.h"

#include "event.h"
#include "eventlistener.h"

enum QuestStatus
{
    INVALID,
    TODO,
    DONE,
    DOING
};

static inline QuestStatus questStatusByInt(int status)
{
    if (status == 0)
        return TODO;
    else if (status == 1)
        return DONE;
    else if (status == 2)
        return DOING;
    else
        return INVALID;
}

struct QuestInfo
{
    unsigned id;
    std::string summary;
    std::string description;
    QuestStatus status;
};

class QuestListModel : public gcn::ListModel, public EventListener
{
    public:
        QuestListModel()
        { listen(Event::QuestLogChannel); }

        virtual int getNumberOfElements()
        { return mQuests.size(); }

        virtual std::string getElementAt(int i)
        { return 0; }

        void addQuestInfo(QuestInfo *info)
        { mQuests.push_back(info); }

        QuestInfo *getQuestInfoById(unsigned id)
        {
            for (std::vector<QuestInfo*>::iterator it = mQuests.begin(),
                    it_end = mQuests.end(); it != it_end; ++it)
            {
                if ((*it)->id == id)
                    return (*it);
            }
            return 0;
        }

        QuestInfo *getQuestInfo(unsigned id)
        { return mQuests.at(id); }

        void event(Event::Channel channel, const Event &event);

    private:
        std::vector<QuestInfo*> mQuests;
};

class QuestListBox : public ListBox
{
    public:
        QuestListBox(gcn::ListModel *listModel):
            ListBox(listModel)
        { }

        virtual unsigned getRowHeight(int i) const;

        virtual void drawRow(gcn::Graphics *graphics, int y, int i,
                             int rowHeight);
};

class QuestLogWindow : public Window
{
    public:
        QuestLogWindow();
        virtual ~QuestLogWindow();

    private:
        QuestListModel *mQuestListModel;
        QuestListBox *mQuestListBox;
        ScrollArea *mScrollArea;

};

extern QuestLogWindow *questLogWindow;

#endif /* QUESTLOGWINDOW_H_ */
