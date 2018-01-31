#ifndef CTPDATAREADER_H
#define CTPDATAREADER_H
#include "ctpif_data_fio.h"

class CtpDataInterface : public CtpDataReader
{
public:
    CtpDataInterface(GmdParam* param)
        : CtpDataReader(param) { }

    virtual ~CtpDataInterface() {}
};

#endif // CTPDATAREADER_H
