#include <QApplication>
#include "bll/musicsystem.h"
#include "ui/logindialog.h"
#include "ui/mainwindow.h"
#include "ui/theme.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    Theme::apply(app);

    MusicSystem sys(PROJECT_DIR);
    sys.load();

    LoginDialog login(&sys);
    if (login.exec() != QDialog::Accepted) return 0;

    MainWindow w(&sys);
    w.show();
    return app.exec();
}
