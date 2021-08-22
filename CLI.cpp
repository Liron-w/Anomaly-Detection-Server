/*
 * CLI.cpp
 *
 * Author: Liron Weizman 206505588
 */
#include "CLI.h"

CLI::CLI(DefaultIO *dio)
{
    this->dio = dio;
}

void CLI::start()
{

    vector<Command *> commands;
    Info info;

    UploadFile command1 = UploadFile(this->dio, &info);
    commands.push_back(&command1);
    CurrentCoralation command2 = CurrentCoralation(this->dio, &info);
    commands.push_back(&command2);
    DetectAnomalies command3 = DetectAnomalies(this->dio, &info);
    commands.push_back(&command3);
    DisplayResults command4 = DisplayResults(this->dio, &info);
    commands.push_back(&command4);
    Uploads command5 = Uploads(this->dio, &info);
    commands.push_back(&command5);
    ExitMenu command6 = ExitMenu(this->dio, &info);
    commands.push_back(&command6);

    int i = 0;
    while (i != 6)
    {
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        for (Command *c : commands)
        {
            c->print_description();
        }
        i = stoi(dio->read());
        (commands.at(i - 1))->execute();
    }
}

CLI::~CLI()
{
}
