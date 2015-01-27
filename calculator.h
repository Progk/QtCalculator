#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btn1_clicked();

    void on_btn6_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

    void on_btn4_clicked();

    void on_btn5_clicked();

    void on_btn7_clicked();

    void on_btn8_clicked();

    void on_btn9_clicked();

    void on_btn_res_clicked();

    void on_btn_add_clicked();

    void on_btn0_clicked();

    void on_btn_sub_clicked();

    void on_btn_mul_clicked();

    void on_btn_div_clicked();

    void on_btn_back_space_clicked();

    void on_btn_clear_clicked();

    void on_btn_sqr_clicked();

    void on_btn_rev_clicked();

    void on_btn_dot_clicked();

    void on_btn_clear_e_clicked();

    void on_btn_history_clicked();

    void on_btn_m_plus_clicked();

    void on_btn_ms_clicked();

    void on_btn_mr_clicked();

    void on_btn_mc_clicked();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void on_exitButton_clicked();

    void on_infoButton_clicked();

    void on_exp_text_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QPoint offset;
};

#endif // MAINWINDOW_H
