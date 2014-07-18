//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelsPanel.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the contents of the primary window of the channel manager
//              application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
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

#include "ChannelsPanel.h"
#include "ChannelContainer.h"
#include "ChannelEntry.h"

#include "ODEnableLogging.h"
#include "ODLogging.h"

#include "M+MAdapterChannel.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wc++11-extensions"
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wshadow"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wweak-vtables"
#endif // defined(__APPLE__)
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the contents of the primary window of the channel manager
 application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The name of the font to be used for text. */
static const char * kFontName = "Courier New";

/*! @brief The size of the font to be used for text. */
static const float kFontSize = 15.0;

/*! @brief The line width for a normal connection. */
static const float kNormalConnectionWidth = 2;

/*! @brief The line width for a normal connection. */
static const float kServiceConnectionWidth = (2 * kNormalConnectionWidth);

/*! @brief The initial thickness of the horizontal and vertical scrollbars. */
static const int kDefaultScrollbarThickness = 16;

/*! @brief The initial height of the displayed region. */
static const int kInitialPanelHeight = 400;

/*! @brief The initial width of the displayed region. */
static const int kInitialPanelWidth = 500;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Determine whether a connection can be made, based on the port protocols.
 @param sourceProtocol The protocol of the source port.
 @param destinationProtocol The protocol of the destination port.
 @returns @c true if the protocols permit a connection to be made and @c false
 otherwise. */
static bool protocolsMatch(const String & sourceProtocol,
                           const String & destinationProtocol)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S2s("sourceProtocol = ", sourceProtocol.toStdString(), "destinationProtocol = ", //####
               destinationProtocol.toStdString()); //####
    bool result = false;
    
    if (0 == destinationProtocol.length())
    {
        result = true;
    }
    else
    {
        result = (sourceProtocol == destinationProtocol);
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // protocolsMatch

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ChannelsPanel::ChannelsPanel(void) :
    inherited(), _knownPorts(), _knownEntities(), _defaultBoldFont(), _defaultNormalFont(),
    _horizontalScrollBar(NULL), _verticalScrollBar(NULL), _innerPanel(new Component),
    _scrollbarThickness(kDefaultScrollbarThickness)
#if 0
_entities1(), _entities2(),
,
_foregroundLock(), _backgroundEntities(&_entities1), _foregroundEntities(&_entities2),
_backgroundPorts(&_ports1), _randomizer(Time::currentTimeMillis()), _movementActive(false)
#endif // 0
{
    OD_LOG_ENTER(); //####
    _defaultBoldFont = new Font(kFontName, kFontSize, Font::bold);
    _defaultNormalFont = new Font(kFontName, kFontSize, Font::plain);
    _horizontalScrollBar = new ScrollBar(false);
    _verticalScrollBar = new ScrollBar(true);
    _innerPanel->setSize(kInitialPanelWidth, kInitialPanelHeight);
    addAndMakeVisible(_innerPanel);
    setSize(kInitialPanelWidth, kInitialPanelHeight);
    addAndMakeVisible(_verticalScrollBar);
    _verticalScrollBar->setSingleStepSize(1.0);
    addAndMakeVisible(_horizontalScrollBar);
    _horizontalScrollBar->setSingleStepSize(1.0);
    OD_LOG_EXIT(); //####
} // ChannelsPanel::ChannelsPanel

ChannelsPanel::~ChannelsPanel(void)
{
    OD_LOG_OBJENTER(); //####
    clearOutData();
    _defaultBoldFont = nullptr;
    _defaultNormalFont = nullptr;
    _horizontalScrollBar = nullptr;
    _verticalScrollBar = nullptr;
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::~ChannelsPanel

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void ChannelsPanel::addEntity(ChannelContainer * anEntity)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("anEntity = ", anEntity); //####
    _knownEntities.push_back(anEntity);
    _innerPanel->addChildComponent(anEntity);
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::addEntity

void ChannelsPanel::clearAllVisitedFlags(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            anEntity->clearVisited();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::clearAllVisitedFlags

void ChannelsPanel::clearOutData(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            delete anEntity;
        }
    }
    _innerPanel->removeAllChildren();
    _knownPorts.clear();
    _knownEntities.clear();
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::clearOutData

ChannelContainer * ChannelsPanel::findKnownEntity(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name.toStdString()); //####
    ChannelContainer * result = NULL;
    
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity && (name == anEntity->getName()))
        {
            result = anEntity;
            break;
        }
        
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelsPanel::findKnownEntity

ChannelContainer * ChannelsPanel::findKnownEntityForPort(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name.toStdString()); //####
    PortEntryMap::const_iterator match(_knownPorts.find(name));
    ChannelContainer *           result = NULL;
    
    if (_knownPorts.end() != match)
    {
        for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it;
             ++it)
        {
            ChannelContainer * anEntity = *it;
            
            if (anEntity && anEntity->hasPort(match->second))
            {
                result = anEntity;
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelsPanel::findKnownEntityForPort

ChannelContainer * ChannelsPanel::findKnownEntityForPort(const ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    ChannelContainer * result = NULL;
    
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it;
         ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity && anEntity->hasPort(aPort))
        {
            result = anEntity;
            break;
        }
        
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelsPanel::findKnownEntityForPort

ChannelEntry * ChannelsPanel::findKnownPort(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name.toStdString()); //####
    ChannelEntry *               result = NULL;
    PortEntryMap::const_iterator match(_knownPorts.find(name));
    
    if (_knownPorts.end() == match)
    {
        result = NULL;
    }
    else
    {
        result = match->second;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelsPanel::findKnownPort

void ChannelsPanel::forgetPort(ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    if (aPort)
    {
        PortEntryMap::iterator match(_knownPorts.find(aPort->getName()));
        
        if (_knownPorts.end() != match)
        {
            _knownPorts.erase(match);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::forgetPort

ChannelContainer * ChannelsPanel::getEntity(const size_t index)
const
{
    OD_LOG_OBJENTER(); //####
    ChannelContainer * result;
    
    if (_knownEntities.size() > index)
    {
        result = _knownEntities.at(index);
    }
    else
    {
        result = NULL;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelsPanel::getEntity

size_t ChannelsPanel::getNumberOfEntities(void)
const
{
    OD_LOG_OBJENTER(); //####
    int result = _knownEntities.size();
    
    OD_LOG_OBJEXIT_L(result); //####
    return result;
} // ChannelsPanel::getNumberOfEntities

void ChannelsPanel::paint(Graphics & gg)
{
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
#endif // 0
    // Set up a gradient background, using a radial gradient from the centre to the furthest edge.
    int            hh = getHeight();
    int            ww = getWidth();
    ColourGradient theGradient(Colours::white, ww / 2.0, hh / 2.0, Colours::grey,
                               (hh > ww) ? 0 : ww, (hh > ww) ? hh : 0, true);
    FillType       theBackgroundFill(theGradient);
    
    gg.setFillType(theBackgroundFill);
    gg.fillAll();
    //drawContainers(gg);
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
} // ChannelsPanel::paint

void ChannelsPanel::rememberPort(ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    if (aPort)
    {
        _knownPorts.insert(PortEntryMap::value_type(aPort->getPortName(), aPort));
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::rememberPort

void ChannelsPanel::removeUnvisitedEntities(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity && (! anEntity->wasVisited()))
        {
            _innerPanel->removeChildComponent(anEntity);
            *it = NULL;
            delete anEntity;
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::removeUnvisitedEntities

void ChannelsPanel::resized(void)
{
    OD_LOG_OBJENTER(); //####
    // This is called when the ChannelsPanel is resized.
    // If you add any child components, this is where you should update their positions.
    const int visibleHeight = getHeight() - _scrollbarThickness;
    const int visibleWidth = getWidth() - _scrollbarThickness;
    
    _innerPanel->setBounds(0, 0, visibleWidth, visibleHeight);
    _verticalScrollBar->setBounds(visibleWidth, 0, _scrollbarThickness, visibleHeight);
    _horizontalScrollBar->setBounds(0, visibleHeight, visibleWidth, _scrollbarThickness);
    updateScrollBars();
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::resized

void ChannelsPanel::updateScrollBars(void)
{
    OD_LOG_OBJENTER(); //####
    /*
     _verticalScrollBar->setRangeLimits(0, jmax (document.getNumLines(), firstLineOnScreen + linesOnScreen));
     _verticalScrollBar->setCurrentRange(firstLineOnScreen, linesOnScreen);
     
     _horizontalScrollBar->setRangeLimits(0, jmax ((double) document.getMaximumLineLength(), xOffset + columnsOnScreen));
     _horizontalScrollBar->setCurrentRange(xOffset, columnsOnScreen);*/
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::updateScrollBars




#if 0
void ChannelsPanel::clearOutBackgroundData(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_backgroundEntities->begin());
         _backgroundEntities->end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            delete anEntity;
        }
    }
    _backgroundEntities->clear();
    // Note that the ports will have been deleted by the deletion of the entities.
    _backgroundPorts->clear();
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::clearOutBackgroundData
#endif // 0

#if 0
void ChannelsPanel::clearOutForegroundData(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_foregroundEntities->begin());
         _foregroundEntities->end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            delete anEntity;
        }
    }
    _backgroundEntities->clear();
    _foregroundPorts->clear();
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::clearOutForegroundData
#endif // 0

#if 0
void ChannelsPanel::drawConnections(Graphics & gg)
{
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
#endif // 0
    
#if 0
    // to draw Bezier curves -
    Path myPath;
    
    myPath.startNewSubPath (10.0f, 10.0f);          // move the current position to (10, 10)
    myPath.lineTo (100.0f, 200.0f);                 // draw a line from here to (100, 200)
    myPath.quadraticTo (0.0f, 150.0f, 5.0f, 50.0f); // draw a curve that ends at (5, 50)
    myPath.closeSubPath();                          // close the subpath with a line back to (10, 10)
    
    // add an ellipse as well, which will form a second sub-path within the path..
    myPath.addEllipse (50.0f, 50.0f, 40.0f, 30.0f);
    
    // double the width of the whole thing..
    myPath.applyTransform (AffineTransform::scale (2.0f, 1.0f));
    
    // and draw it to a graphics context with a 5-pixel thick outline.
    g.strokePath (myPath, PathStrokeType (5.0f));
    
#endif // 0
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
} // ChannelsPanel::drawConnections
#endif // 0

#if 0
void ChannelsPanel::drawContainers(Graphics & gg)
{
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
#endif // 0
    if (_networkAvailable)
    {
        _foregroundLock.enter();
        for (ContainerList::const_iterator it(_foregroundEntities->begin());
             _foregroundEntities->end() != it; ++it)
        {
            ChannelContainer * anEntity = *it;
            
            if (anEntity && (! anEntity->isSelected()))
            {
                //addChildComponent(anEntity);
                anEntity->paint(gg);
            }
        }
        for (ContainerList::const_iterator it(_foregroundEntities->begin());
             _foregroundEntities->end() != it; ++it)
        {
            ChannelContainer * anEntity = *it;
            
            if (anEntity && anEntity->isSelected())
            {
                //addChildComponent(anEntity);
                anEntity->paint(gg);
            }
        }
        drawConnections(gg);
#if 0
        if (_dragActive)
        {
            if (_firstAddPort)
            {
                _firstAddPort->drawDragLine(_dragXpos, _dragYpos, _addingUDPConnection);
            }
            else
            {
                _dragActive = false;
            }
        }
#endif // 0
        _foregroundLock.exit();
    }
    else
    {
        gg.setFont(Font(36.0f));
        gg.setColour(Colours::yellow);
        gg.drawText("The YARP network is not running", getLocalBounds(), Justification::centred,
                    true);
    }
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
} // ChannelsPanel::drawContainers
#endif // 0

#if 0
void ChannelsPanel::moveEntityToEndOfForegroundList(ChannelContainer * anEntity)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("anEntity = ", anEntity); //####
    if (anEntity->isSelected())
    {
        ContainerList::iterator it(_foregroundEntities->begin());
        
        for ( ; _foregroundEntities->end() != it; ++it)
        {
            if (anEntity == *it)
            {
                _foregroundEntities->erase(it);
                addEntityToForeground(anEntity);
                break;
            }
            
        }
#if 0
        _movementActive = false;
#endif // 0
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::moveEntityToEndOfForegroundList
#endif // 0

#if 0
void ChannelsPanel::swapBackgroundAndForeground(void)
{
    OD_LOG_OBJENTER(); //####
    if (/*(! _firstAddPort) && (! _firstRemovePort) &&*/ (! _movementActive))
    {
        _foregroundLock.enter();
        swap(_backgroundPorts, _foregroundPorts);
        swap(_backgroundEntities, _foregroundEntities);
        _foregroundLock.exit();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelsPanel::swapBackgroundAndForeground
#endif // 0

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

Colour ChannelsPanel::getMarkerColor(void)
{
    return Colours::yellow;
} // ChannelsPanel::getMarkerColor

Colour ChannelsPanel::getNewConnectionColor(void)
{
    return Colours::gold;
} // ChannelsPanel::getNewConnectionColor

float ChannelsPanel::getNormalConnectionWidth(void)
{
    return kNormalConnectionWidth;
} // ChannelsPanel::getNormalConnectionWidth

Colour ChannelsPanel::getOtherConnectionColor(void)
{
    return Colours::orange;
} // ChannelsPanel::getOtherConnectionColor

float ChannelsPanel::getServiceConnectionWidth(void)
{
    return kServiceConnectionWidth;
} // ChannelsPanel::getServiceConnectionWidth

Colour ChannelsPanel::getTcpConnectionColor(void)
{
    return Colours::teal;
} // ChannelsPanel::getTcpConnectionColor

Colour ChannelsPanel::getUdpConnectionColor(void)
{
    return Colours::purple;
} // ChannelsPanel::getUdpConnectionColor
