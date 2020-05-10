/*
 * Projekt ICP
 * Autori: Adam Ševčík - xsevci64, Martin Hiner - xhiner00
*/

#include "myscene.h"
#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

MyScene::MyScene(QObject *parent) :
    QGraphicsScene(parent)
{

}


void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for (auto* item : items(event->scenePos())) {
        if (auto bus = dynamic_cast<QGraphicsEllipseItem*>(item); bus){

            bus->setPen(QPen({Qt::red},3));
                   }

    }

    QGraphicsScene::mousePressEvent(event);
}
