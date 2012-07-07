

#include "questlogwindow.h"

#include "gui.h"
#include "log.h"
#include "utils/gettext.h"

#include "gui/setup.h"

#include "gui/widgets/windowcontainer.h"

#include "resources/theme.h"

void QuestListModel::event(Event::Channel channel, const Event &event)
{
    if (event.getType() == Event::QuestStatus)
    {
        int id = event.getInt("id");
        int status = event.getInt("status");
        // TODO show notification somehow
        // bool notification = event.getBool("notification");
        std::string summary = event.getString("summary");
        std::string description = event.getString("description");

        QuestInfo *info = getQuestInfoById(id);
        if (!info)
        {
            info = new QuestInfo();
            addQuestInfo(info);
        }

        // Sync questinfo with recieved data
        info->id = id;
        info->status = questStatusByInt(status);
        info->summary = summary;
        info->description = description;
        logger->log("Recieved quest status %d", id);
    }
}

unsigned QuestListBox::getRowHeight(int i) const
{
    if (mSelected == i)
        return getFont()->getHeight() * 2;
    else
        return getFont()->getHeight();
}

void QuestListBox::drawRow(gcn::Graphics *graphics, int y, int i,
                           int rowHeight)
{
    const int alpha = (int)(mAlpha * 255.0f);
    const gcn::Color &highlightColor =
            Theme::getThemeColor(Theme::HIGHLIGHT, alpha);
    const gcn::Color &backgroundColor =
            Theme::getThemeColor(Theme::BACKGROUND, alpha);
    const gcn::Color &doingColor =
            Theme::getThemeColor(Theme::QUEST_DOING, alpha);
    const gcn::Color &todoColor =
            Theme::getThemeColor(Theme::QUEST_TODO, alpha);

    gcn::Color bgColor = backgroundColor;

    QuestListModel* model = static_cast<QuestListModel*>(mListModel);
    QuestInfo *info = model->getQuestInfo(i);

    if (info->status == TODO)
        bgColor = todoColor;
    else if (info->status == DOING)
        bgColor = doingColor;

    if (mSelected == i)
    {
        if (info->status != DONE)
        {
            bgColor.r = (bgColor.r + highlightColor.r) / 2;
            bgColor.g = (bgColor.g + highlightColor.g) / 2;
            bgColor.b = (bgColor.g + highlightColor.b) / 2;
        }
        else
        {
            bgColor = highlightColor;
        }
    }

    graphics->setColor(bgColor);
    graphics->fillRectangle(gcn::Rectangle(0, y, getWidth(), rowHeight));

    graphics->setColor(Theme::getThemeColor(Theme::TEXT));
    graphics->setFont(boldFont);
    graphics->drawText(info->summary, 1, y);

    if (i == mSelected)
    {
        // split at \n
        std::vector<std::string> lines;
        int pos = 0;
        int lastPos = 0;
        while ((pos = info->description.find("\n", pos + 1)) != std::string::npos)
        {
            lines.push_back(info->description.substr(lastPos, pos - lastPos));
            logger->log("splitted \"%s\"", info->description.substr(lastPos, pos - lastPos).c_str());
            lastPos = pos;
        }
        // push remaining string into the vector
        lines.push_back(info->description.substr(
                lastPos, info->description.size() - lastPos));

        graphics->setFont(getFont());
        int yCoord = y;
        for (std::vector<std::string>::iterator it = lines.begin(),
             it_end = lines.end(); it != it_end; ++it)
        {
            graphics->drawText(*it, 1, yCoord);
            yCoord += getFont()->getWidth(*it);
        }
    }
}

QuestLogWindow::QuestLogWindow():
    Window(_("Questlog"))
{
    setWindowName("Skills");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(true);
    setDefaultSize(windowContainer->getWidth() - 280, 30, 275, 425);
    setMinHeight(113);
    setMinWidth(240);
    setupWindow->registerWindowForReset(this);

    mQuestListModel = new QuestListModel();
    mQuestListBox = new QuestListBox(mQuestListModel);
    mScrollArea = new ScrollArea(mQuestListBox);
    mScrollArea->setOpaque(false);
    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mScrollArea->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);

    place(0, 0, mScrollArea, 5, 5);

    center();
    loadWindowState();

}

QuestLogWindow::~QuestLogWindow()
{
    delete mScrollArea;
    delete mQuestListBox;
    delete mQuestListModel;
}
