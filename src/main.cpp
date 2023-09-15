#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlListProperty>

#include "headers/datamodellist.h"
#include "headers/datamodeltree.h"
#include "headers/datamodeltable.h"
#include "headers/connector.h"
#include "headers/sortmodel.h"

static void registerType()
{
    qmlRegisterType<DataModelTable>( "modelTable", 2, 1, "DataModelTable" );
}

Q_COREAPP_STARTUP_FUNCTION( registerType )

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );

    QGuiApplication app( argc, argv );
    QQmlApplicationEngine engine;

    DataModelList *model_list = new DataModelList();
    DataModelTree *model_tree = new DataModelTree();
    SortModel *model_proxy_sort = new SortModel();

    model_proxy_sort->setSourceModel( model_list );
    model_proxy_sort->setSortRole( ROLE_NAME );
    model_proxy_sort->setFilterRole( ROLE_NAME );

    engine.rootContext()->setContextProperty( "model_list", model_list );
    engine.rootContext()->setContextProperty( "model_proxy_sort", model_proxy_sort );
    engine.rootContext()->setContextProperty( "model_tree", model_tree );

    const QUrl url( QStringLiteral( "qrc:/main.qml" ) );
    QObject::connect( &engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit( -1 );
        }
    }, Qt::QueuedConnection );
    engine.load( url );

    return app.exec();
}

