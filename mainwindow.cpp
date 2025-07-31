#include "MainWindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QMessageBox>
#include <cstring>
#include <QPainter>
#include <QPushButton>
#include <cstdio>
#include <QToolBar>

#ifdef __cplusplus
extern "C"{
#include "EventDealer.h"
#endif
#ifdef __cplusplus
}
#endif


Contest Main;
int op = 0;
//define submitdialog
SubmitDialog::SubmitDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Submit Event");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    // User ID input
    userIdEdit = new QLineEdit(this);
    formLayout->addRow("User ID:", userIdEdit);

    // Problem name input
    problemNameEdit = new QLineEdit(this);
    formLayout->addRow("Problem ID:", problemNameEdit);

    // Submit time input
    submitTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    formLayout->addRow("Submit Time:", submitTimeEdit);

    // Status selection
    statusComboBox = new QComboBox(this);
    statusComboBox->addItems({"AC", "WA", "TLE", "RE", "CE", "Others"});
    connect(statusComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SubmitDialog::onStatusChanged);
    formLayout->addRow("Status:", statusComboBox);

    // Time and memory input (initially hidden)
    timeSpinBox = new QSpinBox(this);
    timeSpinBox->setRange(0, 10000); // Set time range
    timeSpinBox->setSuffix(" ms");
    formLayout->addRow("Time:", timeSpinBox);
    timeSpinBox->setVisible(false);

    memorySpinBox = new QSpinBox(this);
    memorySpinBox->setRange(0, 10000); // Set memory range
    memorySpinBox->setSuffix(" MB");
    formLayout->addRow("Memory:", memorySpinBox);
    memorySpinBox->setVisible(false);

    mainLayout->addLayout(formLayout);

    // Submit button
    QPushButton *submitBtn = new QPushButton("Submit", this);
    connect(submitBtn, &QPushButton::clicked, this, &SubmitDialog::onSubmit);
    mainLayout->addWidget(submitBtn);

    setLayout(mainLayout);

    // Initially show time and memory input if status is AC
    onStatusChanged(statusComboBox->currentIndex());
}
//to get data from submit dialog
QString SubmitDialog::getUserId() const
{
    return userIdEdit->text();
}

QString SubmitDialog::getProblemId() const
{
    return problemNameEdit->text();
}

QDateTime SubmitDialog::getSubmitTime() const
{
    return submitTimeEdit->dateTime();
}
QString SubmitDialog::getStatus() const
{
    return statusComboBox->currentText();
}

int SubmitDialog::getTime() const
{
    return timeSpinBox->value();
}

int SubmitDialog::getMemory() const
{
    return memorySpinBox->value();
}

void SubmitDialog::onSubmit()
{
    accept(); // Close the dialog and accept the input
}
//hide the time/memory
void SubmitDialog::onStatusChanged(int index)
{
    bool isAC = (index == 0); // Check if status is AC
    timeSpinBox->setVisible(isAC);
    memorySpinBox->setVisible(isAC);
}
//input questions
QuestionInputDialog::QuestionInputDialog(QWidget *parent) : QDialog(parent) {
    if(!op) setWindowTitle(tr("Create Question"));
    else setWindowTitle(tr("Create User"));
    questionNameEdit = new QLineEdit(this);
    questionNumberEdit = new QLineEdit(this);
    layout = new QFormLayout(this);
    if(!op) {
        layout->addRow(tr("Question Name:"), questionNameEdit);
        layout->addRow(tr("Question Number:"), questionNumberEdit);
    }else {
        layout->addRow(tr("User Name:"), questionNameEdit);
        layout->addRow(tr("User ID:"), questionNumberEdit);
    }
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
//get data from Question input dialog
const char* QuestionInputDialog::getQuestionName() {
    QString InputString = questionNameEdit->text();
    QByteArray latin1Bytes = InputString.toLatin1();
    const char *asciiString = latin1Bytes.constData();
    return asciiString;
}

int QuestionInputDialog::getQuestionNumber(){
    bool ok;
    int number = questionNumberEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please enter a valid question number."));
        return -1; // Return an error code or handle it as needed
    }
    return number;
}
//list widget clicked
void MainWindow::onItemClicked() {
    QString text = listWidget->currentItem()->text();
    if(text[0] == 'N') {
        setting s;
        int i = text.indexOf('.');
        int j = text.indexOf(' ');
        int problemId = text.mid(i+1, j - i - 1).toInt();
        s.problem = problemId;
        s.status = 1;
        st.push_back(s);
        backaction -> setEnabled(true);
        display();
    }
}
//construct mainwindow
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    //load files
    FILE* file1 = fopen("User.dat", "r");
    FILE* file2 = fopen("Problem.dat", "r");
    FILE* file3 = fopen("Submits.dat", "r");
    int size_user, size_problem, size_submits;
    fread(&size_user, sizeof(int), 1, file1);
    for(int i = 0; i < size_user; i++) {
        User *x = (User *)malloc(sizeof(User));
        fread(x -> userName, sizeof(char), 20, file1);
        fread(&x -> UserId, sizeof(int), 1, file1);
        fread(&x -> Submits, sizeof(int), 1, file1);
        fread(&x -> Acs, sizeof(int), 1, file1);
        AddUser(x, &Main);
    }
    fclose(file1);
    fread(&size_problem, sizeof(int), 1, file2);
    for(int i = 0; i < size_problem; i++) {
        Problem *x = (Problem *)malloc(sizeof(Problem));
        fread(x -> problemName, sizeof(char), 20, file2);
        fread(&x -> problemId, sizeof(int), 1, file2);
        fread(&x -> Submits, sizeof(int), 1, file2);
        AddProblem(x, &Main);
    }
    fclose(file2);
    fread(&size_submits, sizeof(int), 1, file3);
    for(int i = 0; i < size_submits; i++) {
        SubmitEvent *x = (SubmitEvent *)malloc(sizeof(SubmitEvent));
        fread(&x -> user, sizeof(int), 1, file3);
        fread(&x -> problem, sizeof(int), 1, file3);
        fread(&x -> status, sizeof(int), 1, file3);
        fread(&x -> year, sizeof(int), 1, file3);
        fread(&x -> month, sizeof(int), 1, file3);
        fread(&x -> day, sizeof(int), 1, file3);
        fread(&x -> hour, sizeof(int), 1, file3);
        fread(&x -> minute, sizeof(int), 1, file3);
        fread(&x -> second, sizeof(int), 1, file3);
        if(x -> status == AC) {
            fread(&x -> timeUsage, sizeof(int), 1, file3);
            fread(&x -> memoryUsage, sizeof(int), 1, file3);
        }else {
            x -> timeUsage = NA;
            x -> memoryUsage = NA;
        }
        x -> submitTime = size_submits - i;
        AddSubmitEvent(x, &Main);
    }
    fclose(file3);
    //window settings
    setting a;
    a.problem = 0;
    a.find = 0;
    a.status = 0;
    a.user = 0;
    st.push_back(a);//window stack
    listWidget = new QListWidget(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(listWidget);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
    layout = new QVBoxLayout(this);
    setCentralWidget(new QWidget(this));
    QWidget *buttonWidget = new QWidget(this);
    layout->addWidget(buttonWidget);
    layout->addWidget(listWidget);
    connect(listWidget, QListWidget::clicked, this, &MainWindow::onItemClicked);
    label = new QLabel(this);
    centralWidget()->setLayout(layout);
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    NewMenu = new QMenu("New", this);
    menuBar -> addMenu(NewMenu);
    AddProblemAction = new QAction("Problem", this);
    NewMenu -> addAction(AddProblemAction);
    connect(AddProblemAction, &QAction::triggered, this, &MainWindow::CreateProblemBox);
    OpenMenu = new QMenu("Open", this);
    DeleteMenu = new QMenu("Delete", this);
    menuBar -> addMenu(DeleteMenu);
    QAction *DeleteUserAction = new QAction("User", this);
    DeleteMenu -> addAction(DeleteUserAction); //menu bars and tools
    display();
    connect(DeleteUserAction, &QAction::triggered, this, [this](){
        bool ok;
        int id = QInputDialog::getInt(this,"Input","User ID",0,1,100000000,1,&ok);
        if(!FindUserId(id, &Main)) {
            QMessageBox::critical(this, "Error", "User Not found");
            return;
        }
        if(!ok) return;
        RemoveUser(FindUserId(id, &Main), &Main);
        Array list = FindUserSubmit(id, &Main);
        for(int i = 0; i < list.size; i++) {
            RemoveSubmitEvent((SubmitEvent *)list.a[i], &Main);
            int problem = ((SubmitEvent *)list.a[i])->problem;
            Problem *p = FindProblemId(problem, &Main);
            UpdateProblemSubmit(p, p -> Submits - 1, &Main);
        }
        display();
    });
    QAction *DeleteProblemAction = new QAction("problem", this);
    DeleteMenu -> addAction(DeleteProblemAction);
    connect(DeleteProblemAction, &QAction::triggered, this, [this](){
        bool ok;
        int id = QInputDialog::getInt(this, "Input", "Problem ID", 0, 1, 100000000, 1, &ok);
        if(!ok) return;
        if(!FindProblemId(id, &Main)) {
            QMessageBox::critical(this, "Error", "Problem not found!");
            return;
        }
        RemoveProblem(FindProblemId(id, &Main), &Main);
        Array list = FindProblemSubmit(id, &Main);
        for(int i = 0; i < list.size; i++) {
            int status = ((SubmitEvent *)list.a[i]) -> status;
            RemoveSubmitEvent((SubmitEvent *)list.a[i], &Main);
            int user = ((SubmitEvent *)list.a[i]) -> user;
            User *u = FindUserId(user, &Main);
            u -> Submits--;
            if(status == AC) UpdateUserAc(u, u->Acs-1, &Main);
        }
        display();
    });
    toolBar = addToolBar("");
    backaction = toolBar -> addAction("Back");
    backaction -> setEnabled(false);
    QAction *statusAction = toolBar -> addAction("Status");
    connect(backaction, &QAction::triggered, this, [this](){
        st.pop_back();
        label -> hide();
        if(st.size() == 1) backaction -> setEnabled(false);
        display();
    });
    connect(statusAction, &QAction::triggered, this, [this](){
        setting s;
        backaction -> setEnabled(true);
        s.problem = st[st.size()-1].problem;
        s.status = 1;
        s.user = st[st.size()-1].user;
        s.find = st[st.size()-1].find;
        st.push_back(s);
        display();
    });
    FindMenu = new QMenu("Find", this);
    menuBar -> addMenu(FindMenu);
    QAction *FindProblemAction = new QAction("problem", this);
    FindMenu -> addAction(FindProblemAction);
    QAction *FindUserAction = new QAction("User", this);
    FindMenu -> addAction(FindUserAction);
    connect(FindProblemAction, &QAction::triggered, this, [this](){
        setting a;
        a.find = 1;
        a.problem = 0;
        a.status = 0;
        st.push_back(a);
        display();
        st.pop_back();
    });
    connect(FindUserAction, &QAction::triggered, this, [this](){
        bool ok;
        int id = QInputDialog::getInt(this,"Input","User ID",0,1,100000000,1,&ok);
        if(!ok) return;
        if(FindUserId(id, &Main) == NULL) {
            QMessageBox::critical(this, "Error", "UserId not existed!");
            return;
        }
        if(!st[st.size()-1].status) {
            QMessageBox::warning(this, "Warning", "At this mode we will display all the users in AC number order");
        }
        setting a;
        a.find = 0;
        a.problem = st[st.size()-1].problem;
        a.status = st[st.size()-1].status;
        a.user = id;
        st.push_back(a);
        backaction -> setEnabled(this);
        display();
    });
    SubmitAction = new QAction("Submit", this);
    NewMenu -> addAction(SubmitAction);
    connect(SubmitAction, &QAction::triggered, this, &MainWindow::AddSubmit);
    UserAction = new QAction("User", this);
    NewMenu -> addAction(UserAction);
    connect(UserAction, &QAction::triggered, this, &MainWindow::addUser);
}
// add users
void MainWindow::addUser() {
    //here we use the same function as problems
    op = 1;
    QuestionInputDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        char* userName = const_cast<char *>(dialog.getQuestionName());
        int userNumber = dialog.getQuestionNumber();
        if (userNumber == -1) {
            // Handle error or retry
            return;
        }
        Contest *c = &Main;
        User *x;
        x = (User *)malloc(sizeof(User));
        x -> UserId = userNumber;
        memset(x -> userName, 0, sizeof(x -> userName));
        strcpy(x -> userName, userName);
        x -> Submits = 0;
        x -> Acs = 0;
        if(FindUserId(x -> UserId, c) != NULL) {
            QMessageBox::critical(this, tr("Error"), tr("User Id already exist"));
            return;
        }
        if(FindUserName(x -> userName, c) != NULL) {
            QMessageBox::critical(this, tr("Error"), tr("User Name already exist"));
            return;
        }
        AddUser(x, c);
        // Use the input data to create a question
        char buffer[40];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "User '%s' with number %d created.", x -> userName, x -> UserId);
        QMessageBox::information(this, tr("User Created"), buffer);
    }
}
//add submits 
void MainWindow::AddSubmit() {
    SubmitDialog *submitDialog = new SubmitDialog(this);
    if (submitDialog->exec() == QDialog::Accepted)
    {
        bool ok;
        ll userId = submitDialog->getUserId().toInt(&ok);
        if(!ok) {
            QMessageBox::critical(this, "Error", "Invalid User ID");
            return;
        }
        int problemId = submitDialog->getProblemId().toInt(&ok);
        if(!ok) {
            QMessageBox::critical(this, "Error", "Invalid Problem ID");
            return;
        }
        QDateTime submitTime = submitDialog->getSubmitTime();
        QString status = submitDialog->getStatus();
        int time, memory;
        if(status == "AC") {
            time = submitDialog->getTime();
            memory = submitDialog->getMemory();
        }
        Contest *c = (&Main);
        User *user = FindUserId(userId, c);
        if(user == NULL) {
            QMessageBox::critical(this, "Error", "User ID not existed");
            return;
        }
        Problem *problem = FindProblemId(problemId, c);
        if(problem == NULL) {
            QMessageBox::critical(this, "Error", "Problem Not existed");
            return;
        }
        SubmitEvent *event = (SubmitEvent *)malloc(sizeof(SubmitEvent));
        event -> user = userId;
        event -> problem = problemId;
        if(status == "AC") event -> status = AC;
        else if(status == "WA") event -> status = WA;
        else if(status == "TLE") event -> status = TLE;
        else if(status == "RE") event -> status = RE;
        else if(status == "CE") event -> status = CE;
        else event -> status = Others;
        if(c->SUP.rt == NULL) event -> submitTime = 1;
        else event -> submitTime = c->SUP.rt->size + 1;
        event -> year = submitTime.date().year();
        event -> month = submitTime.date().month();
        event -> day = submitTime.date().day();
        event -> hour = submitTime.time().hour();
        event -> minute = submitTime.time().minute();
        event -> second = submitTime.time().second();
        if(status == "AC") {
            event -> timeUsage = time;
            event -> memoryUsage = memory;
            user -> Submits++;
            UpdateUserAc(user, user -> Acs + 1, c);
        }else {
            event -> timeUsage = NA;
            event -> memoryUsage = NA;
        }
        AddSubmitEvent(event, c);
        UpdateProblemSubmit(problem, problem -> Submits + 1, c);
        display();
    }
}
//display as the top of the stack
void MainWindow::display() {
    if(!listWidget) {
        listWidget = new QListWidget(this);
    }else {
        listWidget->clear();
    }
    Contest *c = &Main;
    Array list;
    if(!st[st.size()-1].problem) {
        //display user submits
        if(st[st.size()-1].user && !st[st.size()-1].status) {
            list = FindUserOverAc(-1, c);
            for(int i = 0; i < list.size; i++) {
                char buffer[40];
                memset(buffer, 0, sizeof(buffer));
                User *x = (User *)list.a[i];
                sprintf(buffer, " NO.%d   %d   %s   Submits: %d   Accepted: %d", i + 1, x -> UserId, x -> userName, x -> Submits, x -> Acs);
                listWidget -> addItem(buffer);
            }
            free(list.a);
            return;
        }
        if(!st[st.size()-1].find && !st[st.size()-1].status) {
            list = FindProblemOverSubmit(-1, c);
            int totalItems = list.size;
            for (int i = 0; i < totalItems; ++i) {
                char buffer[40];
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "NO.%d  %s    %d Submits", ((Problem *)list.a[i])->problemId, ((Problem *)list.a[i])->problemName, ((Problem *)list.a[i])->Submits);
                listWidget->addItem(buffer);
            }
            free(list.a);
            return;
        }
        //display finding results
        if(st[st.size()-1].find == 1) {
            bool ok;
            int id = QInputDialog::getInt(this,"Input","Problem ID",0,0,100000000,1,&ok);
            if(!ok) return;
            if(!FindProblemId(id, c)) {
                QMessageBox::warning(this, "Warning", "Problem Not Found!");
                return;
            }
            Problem *problem = FindProblemId(id, c);
            char buffer[40];
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "NO.%d  %s    %d Submits", problem -> problemId, problem -> problemName, problem -> Submits);
            listWidget->addItem(buffer);
            return;
        }
        if(st[st.size()-1].status && !st[st.size()-1].user) {
            list = FindSubmitEventsAfter(0, c);
        }
        if(st[st.size()-1].status && st[st.size()-1].user) {
            list = FindUserSubmit(st[st.size()-1].user, c);
        }
    }else if(!st[st.size()-1].user){
        list = FindProblemSubmit(st[st.size()-1].problem, c);
    }else list = FindProblemUserSubmit(st[st.size()-1].user, st[st.size()-1].problem, c); 
    //display status
    for(int i = 0; i < list.size; i++) {    
        char buffer[100];
        char buffer1[20];
        memset(buffer, 0, sizeof(buffer));
        memset(buffer1, 0, sizeof(buffer1));
        const char *status = ((SubmitEvent *)list.a[i])->status == AC ? "Accepted":"Unaccepted";
        sprintf(buffer, "%d %s    NO.%d %s   %s", ((SubmitEvent *)list.a[i])->user, FindUserId(((SubmitEvent *)list.a[i])->user, c)->userName, ((SubmitEvent *)list.a[i])->problem, FindProblemId(((SubmitEvent *)list.a[i])->problem, c)->problemName, status);
        if(((SubmitEvent *)list.a[i])->status == AC) {
            sprintf(buffer1, "  %d ms  %dMB", ((SubmitEvent *)list.a[i])->timeUsage, ((SubmitEvent *)list.a[i])->memoryUsage);
        }else{
            sprintf(buffer1, " NA NA");
        }
        char buffer3[30];
        memset(buffer3, 0, sizeof(buffer3));
        SubmitEvent* x = (SubmitEvent *)list.a[i];
        sprintf(buffer3, "  %d/%d/%d %d:%d:%d", x->year, x->month, x->day, x->hour, x->minute, x->second);
        strcat(buffer, buffer1);
        strcat(buffer, buffer3);
        listWidget->addItem(buffer);
    }
    free(list.a);
}
//display question(not used now)
void MainWindow::displayQuestions() {
    if(!listWidget) {
        listWidget = new QListWidget(this);
    }else {
        listWidget->clear();
    }
    Contest *c = &Main;
    Array ProblemList = FindProblemOverSubmit(-1, c);
    int totalItems = ProblemList.size;
   // int totalPages = (totalItems + itemsPerPage - 1) / itemsPerPage;
    for (int i = 0; i < totalItems; ++i) {
        char buffer[40];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "NO.%d  %s %d", ((Problem *)ProblemList.a[i])->problemId, ((Problem *)ProblemList.a[i])->problemName, ((Problem *)ProblemList.a[i])->Submits);
        listWidget->addItem(buffer);
    }
    free(ProblemList.a);
}
//create problems
void MainWindow::CreateProblemBox() {
    op = 0;
    QuestionInputDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        char* questionName = const_cast<char *>(dialog.getQuestionName());
        int questionNumber = dialog.getQuestionNumber();
        if (questionNumber == -1) {
            // Handle error or retry
            return;
        }
        Contest *c = &Main;
        Problem *x;
        x = (Problem *)malloc(sizeof(Problem));
        x -> problemId = questionNumber;
        memset(x -> problemName, 0, sizeof(x -> problemName));
        strcpy(x -> problemName, questionName);
        x -> Submits = 0;
        if(FindProblemId(x -> problemId, c) != NULL) {
            QMessageBox::critical(this, tr("Error"), tr("Problem Id already exist"));
            return;
        }
        if(FindProblemName(x -> problemName, c) != NULL) {
            QMessageBox::critical(this, tr("Error"), tr("Problem Name already exist"));
            return;
        }
        
        AddProblem(x, c);
        display();
        // Use the input data to create a question
        char buffer[40];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "Problem '%s' with number %d created.", x -> problemName, x -> problemId);
        QMessageBox::information(this, tr("Problem Created"), buffer);
    }
}
//delete MainWindow as well as load data into the file
MainWindow::~MainWindow() 
{
    FILE *file1 = fopen("User.dat", "w");
    FILE *file2 = fopen("Problem.dat", "w");
    FILE *file3 = fopen("Submits.dat", "w");
    Array Userlist = FindUserOverAc(-1, &Main);
    Array Problemlist = FindProblemOverSubmit(-1, &Main);
    Array Submitslist = FindSubmitEventsAfter(-1, &Main);
    fwrite(&Userlist.size, sizeof(int), 1, file1);
    for(int i = 0; i < Userlist.size; i++) {
        User *x = (User *)Userlist.a[i];
        fwrite(x -> userName, sizeof(char), 20, file1);
        fwrite(&x -> UserId, sizeof(int), 1, file1);
        fwrite(&x -> Submits, sizeof(int), 1, file1);
        fwrite(&x -> Acs, sizeof(int), 1, file1);
    }
    fclose(file1);
    fwrite(&Problemlist.size, sizeof(int), 1, file2);
    for(int i = 0; i < Problemlist.size; i++) {
        Problem *x = (Problem *)Problemlist.a[i];
        fwrite(x -> problemName, sizeof(char), 20, file2);
        fwrite(&x -> problemId, sizeof(int), 1, file2);
        fwrite(&x -> Submits, sizeof(int), 1, file2);
    }
    fclose(file2);
    fwrite(&Submitslist.size, sizeof(int), 1, file3);
    for(int i = 0; i < Submitslist.size; i++) {
        SubmitEvent *x = (SubmitEvent *)Submitslist.a[i];
        fwrite(&x -> user, sizeof(int), 1, file3);
        fwrite(&x -> problem, sizeof(int), 1, file3);
        fwrite(&x -> status, sizeof(int), 1, file3);
        fwrite(&x -> year, sizeof(int), 1, file3);
        fwrite(&x -> month, sizeof(int), 1, file3);
        fwrite(&x -> day, sizeof(int), 1, file3);
        fwrite(&x -> hour, sizeof(int), 1, file3);
        fwrite(&x -> minute, sizeof(int), 1, file3);
        fwrite(&x -> second, sizeof(int), 1, file3);
        if(x -> status == AC) {
            fwrite(&x -> timeUsage, sizeof(int), 1, file3);
            fwrite(&x -> memoryUsage, sizeof(int), 1, file3);
        }
    }
    fclose(file3);
    DeleteTree(Main.CA.rt);
    DeleteTree(Main.CD.rt);
    DeleteAllElements(Main.CI.rt);
    DeleteTree(Main.PD.rt);
    DeleteTree(Main.PS.rt);
    DeleteAllElements(Main.PI.rt);
    DeleteTree(Main.SPT.rt);
    DeleteTree(Main.SUP.rt);
    DeleteTree(Main.SUT.rt);
    DeleteAllElements(Main.ST.rt);
}