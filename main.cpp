#include <QApplication>
#include "musicsystem.h"
#include "logindialog.h"
#include "mainwindow.h"
#include "theme.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    Theme::apply(app);

    MusicSystem sys;
    sys.load(PROJECT_DIR);

    LoginDialog login(&sys);
    if (login.exec() != QDialog::Accepted) return 0;

    MainWindow w(&sys);
    w.show();
    return app.exec();
}
