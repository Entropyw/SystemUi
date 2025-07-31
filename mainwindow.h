#pragma once
#include <QMainWindow>
#include <QDialog>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QScrollArea>
#include <QListWidget>
#include <QScrollBar>
#include <vector>
#include <QPixmap>

#ifdef __cplusplus
extern "C"{
#include "EventDealer.h"
#endif
#ifdef __cplusplus
}
#endif

#include <QComboBox>
#include <QSpinBox>

#include <QComboBox>
#include <QSpinBox>


class SubmitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubmitDialog(QWidget *parent = nullptr);
    QString getUserId() const;
    QString getProblemId() const;
    QDateTime getSubmitTime() const;
    QString getStatus() const;
    int getTime() const;
    int getMemory() const;

private slots:
    void onSubmit();
    void onStatusChanged(int index);

private:
    QLineEdit *userIdEdit;
    QLineEdit *problemNameEdit;
    QDateTimeEdit *submitTimeEdit;
    QComboBox *statusComboBox;
    QSpinBox *timeSpinBox;
    QSpinBox *memorySpinBox;
};

class QuestionInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionInputDialog(QWidget *parent = nullptr);

    const char * getQuestionName();
    int getQuestionNumber();
private:
    QLineEdit *questionNameEdit;
    QLineEdit *questionNumberEdit;
    QDialogButtonBox *buttonBox;
    QFormLayout *layout;
};
struct setting {
    int problem = 0;
    int status = 0;
    int find = 0;
    int user = 0;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    MainWindow(QWidget* parent = nullptr);
    void CreateProblemBox();
    void displayQuestions();
    void display();
    void AddSubmit();
    void addUser();
    void onItemClicked();
    ~MainWindow();
private:
    QScrollArea *scrollArea;
    QListWidget *listWidget;
    QVBoxLayout *layout;
    QAction *AddProblemAction;
    QMenu *OpenMenu;
    QMenu *NewMenu;
    QToolBar *toolBar;
    QAction *backaction;
    QAction *SubmitAction;
    QLabel *label;
    QAction *UserAction;
    QMenu *FindMenu;
    QMenu *DeleteMenu;
    std::vector<setting>st;
};
