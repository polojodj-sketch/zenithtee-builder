// (c) ZenithTee. Avoid Freeze — predicts freeze via brute-force input combinations
// and overrides player input to avoid it.
//
// ZenithTee v1.1 Algorithm: "Latest Safe Tick with Engagement Threshold"
//
// Each frame:
//   1. Check danger windows (with current input, without hook).
//   2. If neither is dangerous → release override, return.
//   3. Engagement: if danger is distant (> trigger_ticks ahead) → monitor only,
//      don't override yet. Let the player handle it.
//      If danger is imminent (<= trigger_ticks) → proceed.
//   4. Test A "Can I wait 1 more tick?": simulate 1 tick with current input,
//      then brute-force combos on remaining ticks. If ANY combo eliminates danger → wait.
//   5. Test B "Act NOW": brute-force all combos from tick 0. Score by survival
//      time (how long before danger). Apply the best combo if it outlasts current.
//   6. Both tests fail → danger is inevitable, leave player input alone.
//
// Combos: direction(-1,0,1) × jump(0,1) × hook(0,1) × aim angles.
// Aim angles are centered on current aim, spread evenly over the FOV cone.

#ifndef GAME_CLIENT_COMPONENTS_KINETIX_AVOID_FREEZE_H
#define GAME_CLIENT_COMPONENTS_KINETIX_AVOID_FREEZE_H

#include <game/client/component.h>
#include <game/client/prediction/gameworld.h>
#include <generated/protocol.h>

class CCollision;
class CCharacter;

class CAvoidFreeze : public CComponent
{
public:
        CAvoidFreeze() = default;
        ~CAvoidFreeze() override = default;

        int Sizeof() const override { return sizeof(*this); }
        void OnReset() override;
        void OnUpdate() override {}
        // Called from CControls::SnapInput at the very end (after all other
        // input manipulators: bot control, fake aim, laser unfreeze).
        void ApplyOverride();

private:
        // Shared simulation context — the world is copied once per ApplyOverride
        // call and reused for every combo evaluation (no per-combo world copies).
        struct SSimContext
        {
                CGameWorld m_World;
                CCharacter *m_pLocalChar = nullptr;
                CCollision *m_pCol = nullptr;
                int m_LocalId = -1;
                int m_StartTick = 0;
        };

        // Runs the simulation for Ticks ticks with the given input.
        // Returns tick (1-based) when danger first occurs, 0 if none.
        int SimulateDanger(SSimContext &Ctx, const CNetObj_PlayerInput &DelayInput, int Delay, const CNetObj_PlayerInput &ComboInput, int Ticks) const;
        int InputDiff(const CNetObj_PlayerInput &A, const CNetObj_PlayerInput &B) const;

        // Hook release tracking. Set when ApplyOverride forces hook=1;
        // consumed next tick when danger clears so BAF can release the override
        // and let the player's own hook state take over.
        bool m_WasOverriding = false;
        int m_OverrideHook = 0;
};

#endif // GAME_CLIENT_COMPONENTS_KINETIX_AVOID_FREEZE_H