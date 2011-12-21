// $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2010      Joerg Henrichs
//// $Id
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_AI_BASE_CONTROLLER_HPP
#define HEADER_AI_BASE_CONTROLLER_HPP

#include "karts/controller/controller.hpp"
#include "states_screens/state_manager.hpp"

class LinearWorld;
class QuadGraph;
class Track;
class Vec3;

/** A base class for all AI karts. This class basically provides some
 *  common low level functions.
 * \ingroup controller
 */
class AIBaseController : public Controller
{
private:
    /** The minimum steering angle at which the AI adds skidding. Lower values
     *  tend to improve the line the AI is driving. This is used to adjust for
     *  different AI levels.
     */
    float    m_skidding_threshold;
    
protected:
    /** Length of the kart, storing it here saves many function calls. */
    float m_kart_length;

    /** Cache width of kart. */
    float m_kart_width;

    /** Keep a pointer to the track to reduce calls */
    Track       *m_track;

    /** Keep a pointer to world. */
    LinearWorld *m_world;

    /** The current node the kart is on. This can be different from the value
     *  in LinearWorld, since it takes the chosen path of the AI into account
     *  (e.g. the closest point in LinearWorld might be on a branch not
     *  chosen by the AI). */
    int   m_track_node;

    // --- Attributes used by Fuzzy AI ---
    /** Last seen track node to detect node changes. Used by FuzzyAIController*/
    int   m_last_seen_track_node;
    /** The path choice method : 0 = RANDOM, 1 = FUZZY_PATH_CHOICE
     *  TODO set constants */
    unsigned int m_path_choice;
    /** The choices that the Fuzzy AI made for the next forks. A new fork choice
     *  is set when a fork is detected in the look_ahead nodes of the kart */
    std::vector<unsigned int> m_fork_choices;
    
    unsigned int m_look_ahead;
    // ------
    
    /** Which of the successors of a node was selected by the AI. */
    std::vector<int> m_successor_index;
    /** For each node in the graph this list contains the chosen next node.
     *  For normal lap track without branches we always have 
     *  m_next_node_index[i] = (i+1) % size;
     *  but if a branch is possible, the AI will select one option here. 
     *  If the node is not used, m_next_node_index will be -1. */
    std::vector<int> m_next_node_index;
    /** For each graph node this list contains a list of the next X
     *  graph nodes. */
    std::vector<std::vector<int> > m_all_look_aheads;


    virtual void update      (float delta) ;
    virtual unsigned int getNextSector(unsigned int index);
    virtual void  newLap             (int lap);
    float    steerToAngle  (const unsigned int sector, const float angle);
    float    steerToPoint  (const Vec3 &point);
    float    normalizeAngle(float angle);
    void     setSteering   (float angle, float dt);
    void     setSkiddingFraction(float f);
    void     computePath();

public:
             AIBaseController(Kart *kart,
                              StateManager::ActivePlayer *player=NULL,
                              int pathChoiceMethod = 0 /* RANDOM by default */);
    virtual ~AIBaseController() {};
};

#endif

/* EOF */
