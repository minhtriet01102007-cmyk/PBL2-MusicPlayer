#include <QApplication>
#include <QDir>
#include <QFile>
#include <QStyleFactory>
#include <QTextStream>
#include "bll/musicsystem.h"
#include "ui/mainwindow.h"
#include "ui/theme.h"

// Lần chạy đầu tiên chưa có dữ liệu: tạo sẵn một tài khoản Admin để đăng nhập được (admin / admin).
static void ensureFirstRun(const QString& root){
    QDir().mkpath(root + "/data");
    QFile file(root + "/data/admin.txt");
    if (file.exists() && file.size() > 0) return;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out << "U001 | admin | admin@melodia.local | admin\n";
    }
}

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    app.setApplicationName("Melodia");
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setStyleSheet(theme::styleSheet());

    // PROJECT_DIR do CMake truyền vào; nếu thư mục đó không còn thì dùng thư mục chứa file chạy
    QString root = QString::fromUtf8(PROJECT_DIR);
    if (!QDir(root + "/data").exists()) root = QCoreApplication::applicationDirPath();
    ensureFirstRun(root);

    MusicSystem sys(root.toStdString());
    sys.load();

    MainWindow window(sys);
    window.show();
    return app.exec();
}