// (c) ZenithTee. HUD overlay component — Spectator list, Array list,
// Admin Alarm flash. One component renders all ZenithTee screen-space
// overlays so they stack in a predictable order.
//
//   Spectator list (kx_spectator_list) — names of players in the spectator
//   team, drawn top-right (same data the scoreboard uses).
//   Array list (kx_array_list) — classic feature list of all currently
//   enabled ZenithTee modules, drawn top-left.
//   Admin Alarm (kx_admin_alarm) — triggered by CGameClient on auth
//   detection; flashes a red banner for a few seconds.

#ifndef GAME_CLIENT_COMPONENTS_KINETIX_ZENITH_HUD_H
#define GAME_CLIENT_COMPONENTS_KINETIX_ZENITH_HUD_H

#include <game/client/component.h>

class CZenithHud : public CComponent
{
public:
        CZenithHud() = default;
        ~CZenithHud() override = default;

        int Sizeof() const override { return sizeof(*this); }
        void OnReset() override;
        void OnRender() override;

        // Raise the admin alarm overlay (called from CGameClient snapshot
        // processing when a player transitions to server auth).
        void TriggerAdminAlarm();

private:
        float m_AdminAlarmSince = -1.0f;

        void RenderSpectatorList();
        void RenderArrayList();
        void RenderAdminAlarm();
        void RenderServerAim();
};

#endif // GAME_CLIENT_COMPONENTS_KINETIX_ZENITH_HUD_H