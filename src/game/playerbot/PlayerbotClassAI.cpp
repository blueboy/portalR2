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

#include "PlayerbotClassAI.h"
#include "Common.h"

PlayerbotClassAI::PlayerbotClassAI(Player* const master, Player* const bot, PlayerbotAI* const ai) : m_master(master), m_bot(bot), m_ai(ai) {}
PlayerbotClassAI::~PlayerbotClassAI() {}

bool PlayerbotClassAI::DoFirstCombatManeuver(Unit *)
{
    // return false, if done with opening moves/spells
    return false;
}
bool PlayerbotClassAI::DoNextCombatManeuver(Unit *) { return false; }

void PlayerbotClassAI::DoNonCombatActions() {}

bool PlayerbotClassAI::BuffPlayer(Player* target)
{
    return false;
}

bool PlayerbotClassAI::CastSpellNoRanged(uint32 nextAction, Unit *pTarget)
{
    if (!m_ai)  return false;
    if (!m_bot) return false;

    if (nextAction == 0)
        return true; // Asked to do nothing so... yeh... Dooone.

    if (pTarget != NULL)
        return m_ai->CastSpell(nextAction, *pTarget);
    else
        return m_ai->CastSpell(nextAction);
}

bool PlayerbotClassAI::CastSpellWand(uint32 nextAction, Unit *pTarget, uint32 SHOOT)
{
    if (!m_ai)  return false;
    if (!m_bot) return false;

    if (SHOOT > 0 && m_bot->FindCurrentSpellBySpellId(SHOOT) && m_bot->GetWeaponForAttack(RANGED_ATTACK, true, true))
    {
        if (nextAction == SHOOT)
            // At this point we're already shooting and are asked to shoot. Don't cause a global cooldown by stopping to shoot! Leave it be.
            return true; // ... We're asked to shoot and are already shooting so... Task accomplished?

        // We are shooting but wish to cast a spell. Stop 'casting' shoot.
        m_bot->InterruptNonMeleeSpells(true, SHOOT);
        // ai->TellMaster("Interrupting auto shot.");
    }

    // We've stopped ranged (if applicable), if no nextAction just return
    if (nextAction == 0)
        return true; // Asked to do nothing so... yeh... Dooone.

    if (nextAction == SHOOT)
    {
        if (SHOOT > 0 && m_ai->GetCombatStyle() == PlayerbotAI::COMBAT_RANGED && !m_bot->FindCurrentSpellBySpellId(SHOOT) && m_bot->GetWeaponForAttack(RANGED_ATTACK, true, true))
            return m_ai->CastSpell(SHOOT, *pTarget);
        else
            // Do Melee attack
            return false; // We're asked to shoot and aren't.
    }

    if (pTarget != NULL)
        return m_ai->CastSpell(nextAction, *pTarget);
    else
        return m_ai->CastSpell(nextAction);
}
