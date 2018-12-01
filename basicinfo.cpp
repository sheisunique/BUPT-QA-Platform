#include "basicinfo.h"

BasicInfo::BasicInfo(QString creator_id, QString content)
{
    this->creator_id = creator_id;
    this->content = content;
    this->create_time = QDateTime::currentDateTime();
    //此id用创建时间的秒数来表示，该秒数为1970-01-01这天到现在的秒数
    this->id = QString("%1").arg(this->create_time.toTime_t());
}
