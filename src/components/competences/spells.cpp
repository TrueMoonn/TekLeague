/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** spells.cpp
*/

#include <print>
#include <vector>

#include "components/competences/spells.hpp"

void registerSpellCasting(Game& game) {
    game.registerComponent<Spell>("spell",
        [&game](ECS::Entity e, const toml::table& params) {
        const std::string& name = params["name"].value_or("unkown");
        std::size_t base = params["base"].value_or(0);
        std::size_t dtype = params["damage_type"].value_or(0);
        std::size_t mc = params["mana_cost"].value_or(0);
        std::size_t spd = params["speed"].value_or(0);
        std::size_t r = params["range"].value_or(0);
        std::array<std::size_t, DamageType::DMGLIMIT> ratios;
        ratios.fill(0);
        if (params["ratios"].is_array()) {
            const auto& raw_ratios = params["ratios"].as_array();
            for (std::size_t i = 0; i < raw_ratios->size(); ++i)
                ratios[i] = raw_ratios->at(i).value_or(0);
        }
        float cd = params["cooldown"].value_or(1.0);
        float ctime = params["cast_time"].value_or(0.5);
        std::size_t target = params["target"].value_or(SpellTarget::ANY);
        game.createComponent<Spell>(e, name, base,
            static_cast<EffectType>(dtype), ratios, cd, ctime,
            static_cast<SpellTarget>(target), mc, spd, r);
    });
    game.registerComponent<Spells>("spells",
        [&game](ECS::Entity e, const toml::table& params) {
        std::vector<std::size_t> spells;
        if (params["indexs"].is_array()) {
            const auto& raw_spells = params["indexs"].as_array();
            for (std::size_t idx = 0; idx < raw_spells->size(); ++idx)
                spells.push_back(raw_spells->at(idx).value_or(0));
        }
        game.createComponent<Spells>(e, spells);
    });
}
