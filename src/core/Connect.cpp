

#include <stdlib.h>
#include <QDebug>

#include "common.h"
#include "Connect.h"
#include "mainwindowpockemul.h"
#include "pobject.h"

#ifdef P_AVOID
#include "libavoid.h"
#endif

extern MainWindowPockemul* mainwindow;

Cconnector::Cconnector(CPObject *parent , QString desc, bool newGender)
{
    Parent	= parent;
    Desc = desc;
    gender = newGender;
    values = 0;
}

Cconnector::Cconnector(CPObject *parent , int nb, int id, ConnectorType type, QString desc, bool newGender, QPoint snap, ConnectorDir dir)
{
    this->Parent = parent;
    this->Desc   = desc;
    this->gender = newGender;
    this->values = 0;
    this->nbpins = nb;
    this->Id     = id;
    this->Type   = type;
    setSnap(snap);
    this->dir    = dir;

}

bool Cconnector::arePluggable(Cconnector *a,Cconnector *b) {
    return ( (a->nbpins==b->nbpins) && (a->gender != b->gender) && (a->Type == b->Type) );
}

// Link current connector to Source connector
void Cconnector::ConnectTo(Cconnector * source)
{
	if (!source) return;
    values = source->values;
//    qWarning()<<source->Parent->getcfgfname()<<":"<<source->Id<<source->Desc<<" copied to "<<this->Parent->getcfgfname()<<":"<<Id<<Desc;
}

void Cconnector::setSnap(QPoint p)
{
    snap = p;

    double x = (double)p.x()/Parent->getDX();
    double y = (double)p.y()/Parent->getDY();
#ifdef P_AVOID
    if (mainwindow->shapeRefList.contains(Parent)) {
        shapeconnectionpin = new Avoid::ShapeConnectionPin(mainwindow->shapeRefList[Parent],
                                                           Id+1,
                                                           x,y,
                                                           10);

    }
#endif
}

void Cconnector::Dump_pin(void)
{
	for (int i=0;i<nbpins;i++)
	{
		if ((values >>i) & 0x01)	dump[i] = '1';
		else						dump[i] = '0';
	}
	dump[nbpins]='\0';

}

bool Cconnector::getGender()
{
	return gender;
}

void Cconnector::setGender(bool newgender)
{
	gender = newgender;
}

Cconnector::ConnectorType Cconnector::getType()
{
    return this->Type;
}

void Cconnector::setType(Cconnector::ConnectorType type)
{
    this->Type = type;
}

QPoint Cconnector::pos() {
    return Parent->pos() + snap*mainwindow->zoom/100;
}
