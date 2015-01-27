#include "calculator.h"
#include "ui_calculator.h"

#include <string>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QVector>
#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QMessageBox>
#include <QKeyEvent>


QString exp; //result
QString exp_add; //last added value
QString exp_new; //new value
QString str; //string QLineEdit
QChar last_oper; //last operation
QString memory; //mem
QStringList results; //history
int results_n = 0; //count history
bool result_state; //statee for result
bool operation; //push operation protect


typedef struct
{
    long long chisl;
    long long znam;
}drob_t;


drob_t to_drob_t(QString const value)
{
    drob_t val;
    if ( value.contains(QRegExp("[|]") ) )
    {
        QStringList list = value.split(QRegExp("[|]"));
        val.chisl = list[0].toLongLong();
        val.znam = list[1].toLongLong();
    }
    else
    {
        val.chisl = value.toLongLong();
        val.znam = 1;
    }
    return val;
}

QString evaluate(QString const &result, QString const &value, QChar operation) //evaluate result and value
{
    if ( result.isEmpty() )
        return QString(value);
    if ( value.isEmpty() )
        return QString(result);

    drob_t val;
    drob_t res;
    val =  to_drob_t(value);
    res = to_drob_t(result);

    switch (operation.toLatin1())
    case '+':
    {
        if ( val.znam == res.znam )
        {
            res.chisl += val.chisl;
        }
        else
        {
            res.chisl = res.chisl * val.znam + val.chisl * res.znam;
            res.znam = res.znam * val.znam;
        }
        break;
    case '-':
            if ( val.znam == res.znam )
            {
                res.chisl -= val.chisl;
            }
            else
            {
                res.chisl = res.chisl * val.znam - val.chisl * res.znam;
                res.znam = res.znam * val.znam;
            }
            break;
        case '*':
            res.chisl *= val.chisl;
            res.znam *= val.znam;
            break;
        case '/':
            res.chisl *= val.znam;
            res.znam *= val.chisl;
            break;
        }

        if ( res.znam == 0 || val.znam == 0 )
        {
            return QString("error");
        }
        else if ( res.chisl % res.znam == 0 )
        {
            res.chisl /= res.znam;
            res.znam = 1;
        }

        if ( res.znam == 1 )
            return QString::number(res.chisl);
        else
            return QString::number(res.chisl) + "|" + QString::number(res.znam);
}

QString evaluate_func(QString const &value, QChar func)
{
    drob_t val;
    val = to_drob_t(value);
    if ( func == 's')
    {
        val.chisl *= val.chisl;
        val.znam *= val.znam;
    }
    else
    {
        long long c = val.chisl;
        val.chisl = val.znam;
        val.znam = c;
    }
    if ( val.znam == 1 )
        return QString::number(val.chisl);
    else
        return QString::number(val.chisl) + "|" + QString::number(val.znam);
}

void default_state(Ui::MainWindow *ui)
{
    exp.clear();
    exp_add.clear();
    exp_new.clear();
    last_oper = '+';
    operation = false;
    result_state = false;
    str.clear();
    results.clear();
    results_n = 0;
    ui->check_m->setChecked(false);
    ui->check_m->setCheckable(false);
}

void check_operation(Ui::MainWindow *ui)
{
    exp_add = QString(exp_new);
    exp_new = QString(str);
    exp = evaluate(exp, exp_new, last_oper);
    ui->exp_text->setText(exp);
    if ( exp.compare("error") == 0 )
    {
        default_state(ui);
        return;
    }

    if ( exp_add.isEmpty() )
    {
        exp_add = QString(exp_new);
        exp_new.clear();
    }
    str.clear();
}




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->check_m->setEnabled(false);
    ui->exp_text->setValidator(new QRegExpValidator(QRegExp("^-?[0-9]+[|]?[1-9][0-9]*")));
    ui->exp_text->setPlaceholderText("0");
    ui->exp_text->setAlignment(Qt::AlignRight);
    ui->exitButton->setToolTip("exit");
    ui->infoButton->setToolTip("about");
    ui->btn0->setToolTip("zero"); //help
    ui->btn1->setToolTip("one");
    ui->btn2->setToolTip("two");
    ui->btn3->setToolTip("three");
    ui->btn4->setToolTip("four");
    ui->btn5->setToolTip("five");
    ui->btn6->setToolTip("six");
    ui->btn7->setToolTip("seven");
    ui->btn8->setToolTip("eight");
    ui->btn9->setToolTip("nine");
    ui->btn_add->setToolTip("addition");
    ui->btn_sub->setToolTip("subtraction");
    ui->btn_mul->setToolTip("multiply");
    ui->btn_div->setToolTip("divide");
    ui->btn_sqr->setToolTip("square");
    ui->btn_div->setToolTip("divide");
    ui->btn_rev->setToolTip("reverse");
    ui->btn_res->setToolTip("show result");
    ui->btn_back_space->setToolTip("delete last digit");
    ui->btn_clear->setToolTip("reset");
    ui->btn_clear_e->setToolTip("clear field");
    ui->btn_dot->setToolTip("dot");
    ui->btn_history->setToolTip("show history");
    ui->btn_mc->setToolTip("clear memory");
    ui->btn_mr->setToolTip("show memory");
    ui->btn_ms->setToolTip("save to memory");
    ui->btn_m_plus->setToolTip("add to memory");
    this->setAttribute(Qt::WA_TranslucentBackground); //transparent
    this->setWindowFlags(Qt::FramelessWindowHint); //hide border widget
    default_state(ui);
    ui->exp_text->setFocus();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::on_btn0_clicked()
{
    operation = false;
    result_state = false;
    str.append("0");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn1_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("1");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn2_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("2");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn3_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("3");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn4_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("4");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn5_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("5");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn6_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("6");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn7_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("7");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn8_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("8");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn9_clicked()
{
    operation = false;
    result_state = false;
    exp_new.clear();
    str.append("9");
    ui->exp_text->setText(str);
}

void MainWindow::on_btn_add_clicked()
{ 

    if ( !operation )
        check_operation(ui);
    last_oper = '+';
    operation = true;
    result_state = false;

    ui->exp_text->setFocus();

}


void MainWindow::on_btn_sub_clicked()
{
    if ( !operation )
        check_operation(ui);

    last_oper = '-';
    result_state = false;
    operation = true;
    ui->exp_text->setFocus();
}

void MainWindow::on_btn_mul_clicked()
{
    if ( !operation )
        check_operation(ui);
    last_oper = '*';
    result_state = false;
    operation = true;
    ui->exp_text->setFocus();
}

void MainWindow::on_btn_div_clicked()
{
    if ( !operation )
        check_operation(ui);
    last_oper = '/';
    result_state = false;
    operation = true;
    ui->exp_text->setFocus();
}

void MainWindow::on_btn_dot_clicked()
{
    if ( !str.isEmpty() )
        str.append("|");
    if ( str.count("|") > 1)
        str.chop(1);
    ui->exp_text->setText(str);
    ui->exp_text->setFocus();
}

void MainWindow::on_btn_sqr_clicked()
{
    str = ui->exp_text->text();
    str = evaluate_func(str, 's');
    ui->exp_text->setText(str);
    if ( result_state )
    {
        exp = QString(str);
        str.clear();
    }
    //result_state = false;
    //last_oper = 's';
    ui->exp_text->setFocus();
}

void MainWindow::on_btn_rev_clicked()
{
    str = ui->exp_text->text();
    str = evaluate_func(str, 'r');
    ui->exp_text->setText(str);
    if ( result_state )
    {
        exp = QString(str);
        str.clear();
    }
    //result_state = false;
    //last_oper = 'r';
    ui->exp_text->setFocus();
}


void MainWindow::on_btn_res_clicked() //result
{

    if ( last_oper == 'r' || last_oper == 's' )
    {
        if ( exp.isEmpty() )
            exp = ui->exp_text->text();
        exp = evaluate_func(exp, last_oper);
    }
    else
    {
        if ( operation && !result_state )
        {
            exp_new = QString(exp);
            exp = evaluate(exp, exp, last_oper);
        }
        else
        {
            if ( exp_new.isEmpty() )
                exp_new = QString(str);
            exp = evaluate(exp, exp_new, last_oper);
        }
        results.append(exp);
        operation = true;
        result_state = true;
    }
    ui->exp_text->setText(exp);
    str.clear();
}

void MainWindow::on_btn_back_space_clicked()
{
    str = ui->exp_text->text();
    str.chop(1);
    ui->exp_text->setText(str);
    if ( result_state )
    {
        exp = QString(str);
        str.clear();
    }
    ui->exp_text->setFocus();
}

void MainWindow::on_btn_clear_clicked()
{
    default_state(ui);
    ui->exp_text->clear();
}

void MainWindow::on_btn_clear_e_clicked()
{
    str.clear();
    ui->exp_text->clear();
}


void MainWindow::on_btn_history_clicked()
{
    if ( !results.isEmpty() )
    {
        if ( results_n >= results.size() )
            results_n = 0;
        ui->exp_text->setText(results[results.size() - results_n - 1]);
        results_n++;
    }
}

void MainWindow::on_btn_ms_clicked()
{
    memory = QString(str);
    ui->check_m->setCheckable(true);
    ui->check_m->setChecked(true);
}

void MainWindow::on_btn_m_plus_clicked()
{
    memory = evaluate(memory, str, '+');
    ui->check_m->setCheckable(true);
    ui->check_m->setChecked(true);
}

void MainWindow::on_btn_mr_clicked()
{
    ui->exp_text->setText(memory);
}

void MainWindow::on_btn_mc_clicked()
{
    memory.clear();
    ui->check_m->setChecked(false);
    ui->check_m->setCheckable(false);
}

void MainWindow::on_infoButton_clicked()
{
    QMessageBox::information(this, "About", "<font size=6>Sergey Sivulskiy 3101</font>");
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if (isWindow())
        offset = event->globalPos() - pos();
    else
        offset = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    if (isWindow())
        move(event->globalPos() - offset);
    else
        move(mapToParent(event->pos() - offset));
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    offset = QPoint();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ui->exp_text->setFocus();
    char c = event->key();
    if ( c == 5 ) //enter
    {
        MainWindow::on_btn_res_clicked();
    }
}


void MainWindow::on_exp_text_textEdited(const QString &arg1)
{
    ui->exp_text->setFocus();
    char c = arg1.toStdString().at(arg1.length()-1);

    if ( (c >= '0' && c <= '9') || c == '|' )
    {
        operation = false;
        result_state = false;
    }

    switch (c)
    {
    case Qt::Key_0:
        MainWindow::on_btn0_clicked();
        break;
    case Qt::Key_1:
        MainWindow::on_btn1_clicked();
        break;
    case Qt::Key_2:
        MainWindow::on_btn2_clicked();
        break;
    case Qt::Key_3:
        MainWindow::on_btn3_clicked();
        break;
    case Qt::Key_4:
        MainWindow::on_btn4_clicked();
        break;
    case Qt::Key_5:
        MainWindow::on_btn5_clicked();
        break;
    case Qt::Key_6:
        MainWindow::on_btn6_clicked();
        break;
    case Qt::Key_7:
        MainWindow::on_btn7_clicked();
        break;
    case Qt::Key_8:
        MainWindow::on_btn8_clicked();
        break;
    case Qt::Key_9:
        MainWindow::on_btn9_clicked();
        break;
    case '|':
        MainWindow::on_btn_dot_clicked();
        break;
    }
}
