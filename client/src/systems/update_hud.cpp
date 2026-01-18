/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** update_hud.cpp
*/

#include <ECS/DenseZipper.hpp>
#include <cstdio>
#include <format>
#include "components/champion.hpp"
#include "components/stats/health.hpp"
#include "components/stats/mana.hpp"
#include "interaction/components/player.hpp"
#include "maths/Vector.hpp"
#include "scenes/ingame.hpp"
#include "physic/components/position.hpp"
#include "sfml/components/sprite.hpp"
#include "sfml/components/text.hpp"
#include "sfml/components/window.hpp"

#include "entities.hpp"
#include "client_systems.hpp"

void updateHud(Client& game) {
    game.createSystem("update_hud", [&game](ECS::Registry&){
        const auto winView = game.getComponent<addon::sfml::Window>().
            getComponent(SYSTEM_F).win->getView();
        mat::Vector2f winPos = {
            winView.getCenter().x - winView.getSize().x / 2,
            winView.getCenter().y - winView.getSize().y / 2};
        auto& positions = game.getComponent<addon::physic::Position2>();
        auto& players = game.getComponent<addon::intact::Player>();
        auto& sprites = game.getComponent<addon::sfml::Sprite>();
        auto& texts = game.getComponent<addon::sfml::Text>();
        auto& hps = game.getComponent<Health>();
        auto& manas = game.getComponent<Mana>();

        auto& main_pos = positions.getComponent(MAIN_HUD);
        main_pos.x = winPos.x + 560.f;
        main_pos.y = winPos.y + 950.f;

        auto& cpp_pos = positions.getComponent(CHAMP_PP);
        cpp_pos.x = winPos.x + 600.f;
        cpp_pos.y = winPos.y + 975.f;
        auto& capp_pos = positions.getComponent(CHAMP_ACTUAL_PP);
        capp_pos.x = winPos.x + 610.f;
        capp_pos.y = winPos.y + 985.f;

        auto& cname_pos = positions.getComponent(CHAMP_NAME);
        cname_pos.x = winPos.x + 710.f;
        cname_pos.y = winPos.y + 980.f;

        auto& hpbar_pos = positions.getComponent(HP_BAR);
        hpbar_pos.x = winPos.x + 750.f;
        hpbar_pos.y = winPos.y + 1035.f;
        auto& hpbar_val_pos = positions.getComponent(HP_BAR_VAL);
        hpbar_val_pos.x = winPos.x + 755.f;
        hpbar_val_pos.y = winPos.y + 1040.f;

        auto& mabar_pos = positions.getComponent(MANA_BAR);
        mabar_pos.x = winPos.x + 750.f;
        mabar_pos.y = winPos.y + 1055.f;
        auto& mabar_val_pos = positions.getComponent(MANA_BAR_VAL);
        mabar_val_pos.x = winPos.x + 755.f;
        mabar_val_pos.y = winPos.y + 1060.f;

        for (auto&& [_, hp, mana] : ECS::DenseZipper(players, hps, manas)) {
            std::string strmana = std::format("{}/{}",
                mana.amount, mana.max_amount);
            texts.getComponent(MANA_BAR).setString(strmana);
            auto& sp_mana = sprites.getComponent(MANA_BAR_VAL);
            float mana_ratio = static_cast<float>(mana.amount) / mana.max_amount;
            sp_mana.sp.setScale({490.f / 32.f * mana_ratio, 10.f / 32.f});

            std::string strhp = std::format("{}/{}",
                hp.amount, hp.max_amount);
            texts.getComponent(HP_BAR).setString(strhp);
            auto& sp_hp = sprites.getComponent(HP_BAR_VAL);
            float hp_ratio = static_cast<float>(hp.amount) / hp.max_amount;
            sp_hp.sp.setScale({490.f / 32.f * hp_ratio, 10.f / 32.f});
        }
    }, false);
};
