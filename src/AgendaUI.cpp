#include "../include/AgendaUI.hpp"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

AgendaUI::AgendaUI()
{
    m_agendaService.startAgenda();
}
void AgendaUI::OperationLoop(void)
{
    while(true){
        cout << "-------------------------------------Agenda-------------------------------------" << endl;
        cout << "Action :" << endl;
        cout << "l    - log in Agenda by user name and password" << endl;
        cout << "r    - register an Agenda account" << endl;
        cout << "q    - quit Agenda" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << endl;
        cout << "Agenda : ~$ ";
        string operation;
        cin >> operation;
        cout << endl;
        if (operation == "l"){
            userLogIn();
        }
        else if(operation == "r"){
            userRegister();
        }
       else if (operation == "q"){
            quitAgenda();
            break;
       }
    }
}

void AgendaUI::startAgenda(void)
{
    m_agendaService.startAgenda();
}

string AgendaUI::getOperation()
{
    cout << "--------------------------------Agenda--------------------------------" << endl;
    cout << "Action :" << endl;
    cout << "o   - log out Agenda" << endl;
    cout << "dc  - delete Agenda account" << endl;
    cout << "lu  - list all Agenda user" << endl;
    cout << "cm  - create a meeting" << endl;
    cout << "amp - add meeting partticipator" << endl;
    cout << "rmp - remove meeting participator" << endl;
    cout << "rqm - requsest to quit meeting" << endl;
    cout << "la  - list all meetings" << endl;
    cout << "las - list all sponsor meetings" << endl;
    cout << "lap - list all participate meetings" << endl;
    cout << "qm  - query meeting by title" << endl;
    cout << "qt  - query meeting by time interval" << endl;
    cout << "dm  - delete meeting by title" << endl;
    cout << "da  - delete all meetings" << endl;
    cout << "----------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Agenda@" << m_userName << " :~# ";
    string operation;
    cin >> operation;
    return operation;
}

bool AgendaUI::executeOperation(string t_operation)
{
    if (t_operation == "o")
    {
        userLogOut();
        cout << endl;
        return false;
    }
    else if (t_operation == "dc")
    {
        deleteUser();
        cout << endl;
        return false;
    }
    else if (t_operation == "lu")
    {
        listAllUsers();
        cout << endl;
        return true;
    }
    else if (t_operation == "cm")
    {
        createMeeting();
        cout << endl;
        return true;
    }
    else if (t_operation == "amp"){
        cout << endl;
        cout << "[add participator] [meeting title] [participator username]" << endl;
        cout << "[add participator] ";
        string title, participator;
        cin >> title >> participator;
        cout << "[add participator] ";
        if (m_agendaService.addMeetingParticipator(m_userName, title, participator)){
            cout << "succeed!" << endl;
            cout << endl;
        }
        else cout << "error!" << endl << endl;
    }
    else if (t_operation == "rmp"){
        cout << endl;
        cout << "[remove participator] [meeting title] [participator username]" << endl;
        cout << "[remove participator] ";
        string title, participator;
        cin >> title >> participator;
        cout << "[remove participator] ";
        if (m_agendaService.removeMeetingParticipator(m_userName, title, participator)){
            cout << "succeed!" << endl;
            cout << endl;
        }
        else cout << "error!" << endl << endl;
    }
    else if (t_operation == "rqm"){
        cout << endl;
        cout << "[quit meeting] [meeting title]" << endl;
        cout << "[quit meeting] ";
        string title;
        cin >> title;
        cout << "[quit meeting] ";
        if (m_agendaService.quitMeeting(m_userName, title)){
            cout << "succeed!" << endl;
            cout << endl;
        }
        else cout << "error!" << endl << endl;
    }
    else if (t_operation == "la")
    {
        listAllMeetings();
        cout << endl;
        return true;
    }
    else if (t_operation == "las")
    {
        listAllSponsorMeetings();
        cout << endl;
        return true;
    }
    else if (t_operation == "lap")
    {
        listAllParticipateMeetings();
        cout << endl;
        return true;
    }
    else if (t_operation == "qm")
    {
        queryMeetingByTitle();
        cout << endl;
        return true;
    }
    else if (t_operation == "qt")
    {
        queryMeetingByTimeInterval();
        cout << endl;
        return true;
    }
    else if (t_operation == "dm")
    {
        deleteMeetingByTitle();
        cout << endl;
        return true;
    }
    else if (t_operation == "da")
    {
        deleteAllMeetings();
        cout << endl;
        return true;
    }
    return true;
}

void AgendaUI::userLogIn(void)
{
    cout << "[log in] [user name] [password]" << endl;
    cout << "[log in] ";
    string username, password;
    cin >> username >> password;
    cout << "[log in] ";
    if (m_agendaService.userLogIn(username, password))
    {
        m_userName = username;
        m_userPassword = password;
        cout << "succeed!" << endl;
        while (executeOperation(getOperation()));
    }
    else
    {
        cout << "Password error or user doesn't exist" << endl;
        cout << endl;
    }
}

void AgendaUI::userRegister(void)
{
    cout << "[register] [user name] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string userName, password, email, phone;
    cin >> userName >> password >> email >> phone;
    cout << "[register] ";
    if (m_agendaService.userRegister(userName, password, email, phone))
    {
        m_userName = userName;
        m_userPassword = password;
        cout << "succeed!" << endl;
        cout << endl;
    }
    else
    {
        cout << "This username has been registered!" << endl;
        cout << endl;
    }
}

void AgendaUI::userLogOut(void)
{
    m_userName = "";
    m_userPassword = "";
}

void AgendaUI::quitAgenda(void)
{
    m_agendaService.quitAgenda();
}

void AgendaUI::deleteUser(void)
{
    cout << endl;
    m_agendaService.deleteUser(m_userName, m_userPassword);
    m_userName = "";
    m_userPassword = "";
    cout << "[delete agenda account] succeed!" << endl;
}

void AgendaUI::listAllUsers(void) {
    cout << endl;
    cout << "[list all users]" << endl;
    cout << endl;
    cout << left << setw(20) << "name";
    cout << left << setw(30) << "email";
    cout << left << "phone" << endl;
    list<User> t_userLists = m_agendaService.listAllUsers();
    for (auto &user : t_userLists){
        cout << left << setw(20) << user.getName();
        cout << left << setw(30) << user.getEmail();
        cout << left << user.getPhone() << endl;
    }
}

void AgendaUI::createMeeting(void) {
    cout << endl;
    cout << "[create meeting] [the number of participators]";
    cout << "[create meeting] ";
    int numOfParticipators;
    cin >> numOfParticipators;
    vector<string> participators;
    for (int i = 0; i < numOfParticipators; ++i){
        string participator;
        cout << "[create meeting] [please enter the participator " << i + 1 << " ]" << endl;
        cout << "[create meeting] ";
        cin >> participator;
        participators.push_back(participator);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] ";
    cout << "[end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    string title, startdate, enddate;
    cin >> title >> startdate >> enddate;
    cout << "[create meeting] ";
    if (m_agendaService.createMeeting(m_userName, title, 
            startdate, enddate, participators)){
        cout << "succeed!" << endl;
    }
    else{
        cout << "error!" << endl;
    }
}

void AgendaUI::listAllMeetings(void) {
    cout << endl;
    cout << "[list all meetings]" << endl;
    cout << endl;
    list<Meeting> t_meetingLists = m_agendaService.listAllMeetings(m_userName);
    printMeetings(t_meetingLists);
}

void AgendaUI::listAllSponsorMeetings(void) {
    cout << endl;
    cout << "[list all sponsor meetings]" << endl;
    cout << endl;
    list<Meeting> t_meetingLists = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(t_meetingLists);
}

void AgendaUI::listAllParticipateMeetings(void) {
    cout << endl;
    cout << "[list all participator meetings]" << endl;
    cout << endl;
    list<Meeting> t_meetingLists = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(t_meetingLists);
}

void AgendaUI::queryMeetingByTitle(void) {
    cout << endl;
    cout << "[query meeting] [title]" << endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    list<Meeting> t_meetingLists = m_agendaService.meetingQuery(m_userName, title);
    printMeetings(t_meetingLists);
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << endl;
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] ";
    cout << "[end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] " << endl;
    string startdate, enddate;
    cin >> startdate >> enddate;
    list<Meeting> t_meetingLists = m_agendaService.meetingQuery(m_userName, startdate, enddate);
    printMeetings(t_meetingLists);
}

void AgendaUI::deleteMeetingByTitle(void) {
    cout << endl;
    cout << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    if(m_agendaService.deleteMeeting(m_userName, title)){
        cout << "[delete meeting] succeed!" << endl;
    }
    else cout << "[error] delete meeting fail!" << endl;
}

void AgendaUI::deleteAllMeetings(void) {
    cout << endl;
    m_agendaService.deleteAllMeetings(m_userName);
    cout << "[delete all meeting] succeed!" << endl;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    cout << left << setw(15) << "title";
    cout << left << setw(20) << "sponser";
    cout << left << setw(20) << "start time";
    cout << left << setw(20) << "end time";
    cout << left << "participators" << endl;
    for (auto &meeting : t_meetings){
        cout << left << setw(15) << meeting.getTitle();
        cout << left << setw(20) << meeting.getSponsor();
        cout << left << setw(20) << Date::dateToString(meeting.getStartDate());
        cout << left << setw(20) << Date::dateToString(meeting.getEndDate());
        vector<string> participators = meeting.getParticipator();
        for (int i = 0; i < participators.size(); ++i){
            cout << left << participators[i];
            if (i != participators.size() - 1) cout << ",";
        }
        cout << endl;
    }
}
