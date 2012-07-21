/*
* Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
* Copyright (C) 2012 Playerbot Team
* Copyright (C) 2012 MangosR2
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _PLAYERBOTCLASSAI_H
#define _PLAYERBOTCLASSAI_H

#include "Common.h"
#include "../World.h"
#include "../SpellMgr.h"
#include "../Player.h"
#include "../ObjectMgr.h"
#include "WorldPacket.h"
#include "../Unit.h"
#include "../SharedDefines.h"
#include "PlayerbotAI.h"

class Player;
class PlayerbotAI;

enum JOB_TYPE
{
    JOB_HEAL   = 0x01,
    JOB_TANK   = 0x02,
    JOB_MASTER = 0x04, // Not a fan of this distinction but user (or rather, admin) choice
    JOB_DPS    = 0x08,
    JOB_ALL    = 0x0F  // all of the above
};

struct heal_priority
{
    Player* p;
    uint8 hp;
    JOB_TYPE type;
    heal_priority(Player* pin, uint8 hpin, JOB_TYPE t) : p(pin), hp(hpin), type(t) {}
    // overriding the operator like this is not recommended for general use - however we won't use this struct for anything else
    bool operator<(const heal_priority& a) const { return type < a.type; }
};

class MANGOS_DLL_SPEC PlayerbotClassAI
{
public:
    PlayerbotClassAI(Player * const master, Player * const bot, PlayerbotAI * const ai);
    virtual ~PlayerbotClassAI();

    // all combat actions go here
    virtual CombatManeuverReturns DoFirstCombatManeuver(Unit*);
    virtual CombatManeuverReturns DoNextCombatManeuver(Unit*);

    // all non combat actions go here, ex buffs, heals, rezzes
    virtual void DoNonCombatActions();

    // buff a specific player, usually a real PC who is not in group
    virtual bool BuffPlayer(Player* target);

    // Utilities
    Player* GetMaster() { return m_master; }
    Player* GetPlayerBot() { return m_bot; }
    PlayerbotAI* GetAI() { return m_ai; };

protected:
    CombatManeuverReturns CastSpellNoRanged(uint32 nextAction, Unit *pTarget);
    CombatManeuverReturns CastSpellWand(uint32 nextAction, Unit *pTarget, uint32 SHOOT);
    virtual CombatManeuverReturns HealTarget(Unit* /*target*/) { return RETURN_NO_ACTION_UNKNOWN; }
    virtual Unit* GetHealTarget(JOB_TYPE type = JOB_ALL);
    JOB_TYPE GetTargetJob(Player* target);

    // These values are used in GetHealTarget and can be overridden per class (to accomodate healing spell health checks)
    uint8 m_MinHealthPercentTank;
    uint8 m_MinHealthPercentHealer;
    uint8 m_MinHealthPercentDPS;
    uint8 m_MinHealthPercentMaster;
    uint8 m_MinHealthPercentAll;    // Must be equal to or over highest of all 4 above values

    Player* m_master;
    Player* m_bot;
    PlayerbotAI* m_ai;
};

#endif
