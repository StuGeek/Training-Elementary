#ifndef __AGENDAUI_HPP__
#define __AGENDAUI_HPP__

#include "../include/AgendaService.hpp"
#include <iostream>
#include <string>

using namespace std;

class AgendaUI
{
    public:
        AgendaUI();

        void OperationLoop(void);

    private:
        // task functions
        void startAgenda(void);

        string getOperation();

        bool executeOperation(std::string op);

        void userLogIn(void);

        void userRegister(void);

        void userLogOut(void);

        void quitAgenda(void);

        void deleteUser(void);

        void listAllUsers(void);

        void createMeeting(void);

        void listAllMeetings(void);

        void listAllSponsorMeetings(void);

        void listAllParticipateMeetings(void);

        void queryMeetingByTitle(void);

        void queryMeetingByTimeInterval(void);

        void deleteMeetingByTitle(void);

        void deleteAllMeetings(void);

        void printMeetings(std::list<Meeting> meetings);

        // dates
        std::string m_userName;
        std::string m_userPassword;
        AgendaService m_agendaService;
};

#endif