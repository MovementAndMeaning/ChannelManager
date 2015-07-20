//--------------------------------------------------------------------------------------------------
//
//  File:       m+mManagerWindow.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the primary window of the m+m manager application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by H Plus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2014-07-14
//
//--------------------------------------------------------------------------------------------------

#include "m+mManagerWindow.h"

#include "m+mContentPanel.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the primary window of the m+m manager application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MPlusM_Manager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The colour to be used for the window background. */
static const Colour & kWindowBackgroundColour(Colours::lightgrey);

#if SETTINGS_FOR_MANUAL_
/*! @brief The height of the fixed, small window used when doing screen captures for the manual. */
static const int kWindowHeightForManual = 360;
#endif // SETTINGS_FOR_MANUAL_

#if SETTINGS_FOR_MANUAL_
/*! @brief The width of the fixed, small window used when doing screen captures for the manual. */
static const int kWindowWidthForManual = 640;
#endif // SETTINGS_FOR_MANUAL_

/*! @brief The command manager object used to dispatch command events. */
static ScopedPointer<ApplicationCommandManager> lApplicationCommandManager;

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ManagerWindow::ManagerWindow(const YarpString & title)  :
    inherited1(), inherited2(title.c_str(), kWindowBackgroundColour, inherited2::allButtons),
    _contentPanel(new ContentPanel(this)), _scannerThread(NULL)
{
    OD_LOG_ENTER(); //####
    setOpaque(true);
    setResizable(true, true);
    setContentOwned(_contentPanel, true);
#if SETTINGS_FOR_MANUAL_
    centreWithSize(kWindowWidthForManual, kWindowHeightForManual);
#else // ! SETTINGS_FOR_MANUAL_
    centreWithSize(getWidth(), getHeight());
#endif // ! SETTINGS_FOR_MANUAL_
    setVisible(true);
    addKeyListener(getApplicationCommandManager().getKeyMappings());
    triggerAsyncUpdate();
    OD_LOG_EXIT_P(this); //####
} // ManagerWindow::ManagerWindow

ManagerWindow::~ManagerWindow(void)
{
    OD_LOG_OBJENTER(); //####
    lApplicationCommandManager = NULL;
    OD_LOG_OBJEXIT(); //####
} // ManagerWindow::~ManagerWindow

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void ManagerWindow::closeButtonPressed(void)
{
    OD_LOG_OBJENTER(); //####
    // This is called when the user tries to close this window. Here, we'll just
    // ask the app to quit when this happens, but you can change this to do
    // whatever you need.
    JUCEApplication::getInstance()->systemRequestedQuit();
    OD_LOG_OBJEXIT(); //####
} // ManagerWindow::closeButtonPressed

ApplicationCommandManager & ManagerWindow::getApplicationCommandManager(void)
{
    OD_LOG_ENTER(); //####
    if (! lApplicationCommandManager)
    {
        lApplicationCommandManager = new ApplicationCommandManager;
    }
    OD_LOG_EXIT_P(lApplicationCommandManager); //####
    return *lApplicationCommandManager;
} // ManagerWindow::getApplicationCommandManager

EntitiesPanel & ManagerWindow::getEntitiesPanel(void)
const
{
    OD_LOG_OBJENTER(); //####
    EntitiesPanel & thePanel(_contentPanel->getEntitiesPanel());
    
    OD_LOG_OBJEXIT_P(&thePanel); //####
    return thePanel;
} // ManagerWindow::getEntitiesPanel

void ManagerWindow::handleAsyncUpdate(void)
{
    OD_LOG_OBJENTER(); //####
    ApplicationCommandManager & commandManager = getApplicationCommandManager();

    commandManager.registerAllCommandsForTarget(_contentPanel);
    commandManager.registerAllCommandsForTarget(JUCEApplication::getInstance());
    OD_LOG_OBJEXIT(); //####
} // ManagerWindow::handleAsyncUpdate

void ManagerWindow::setScannerThread(ScannerThread * theScanner)
{
    OD_LOG_OBJENTER(); //####
    _scannerThread = theScanner;
    OD_LOG_OBJEXIT(); //####
} // ManagerWindow::setScannerThread

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
