/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** spells.cpp
*/

#include <array>
#include <print>
#include <vector>

#include "components/competences/spells.hpp"

void registerSpellCasting(Game& game) {
    game.registerComponent<Spells>("spells",
        [&game](ECS::Entity e, const toml::table& params) {
        std::vector<Casting> spells;
        if (params["spells"].is_array_of_tables()) {
            const auto& raw_spells = params["spells"].as_array();
            for (std::size_t idx = 0; idx < raw_spells->size(); ++idx) {
                const auto& spell = raw_spells[idx].as_table();
                const std::string& name = spell->at("name").value_or("unkown");
                std::size_t base = spell->at("base").value_or(0);
                std::size_t dtype = spell->at("damage_type").value_or(0);
                std::size_t mc = spell->at("mana_cost").value_or(0);
                std::array<std::size_t, DamageType::DMGLIMIT> ratios;
                ratios.fill(0);
                if (spell->at("ratios").is_array()) {
                    const auto& raw_ratios = spell->at("ratios").as_array();
                    for (std::size_t i = 0; i < DamageType::DMGLIMIT; ++i)
                        ratios[i] = raw_ratios[i].value_or(0);
                }
                float cd = spell->at("cooldown").value_or(1.0);
                float ctime = spell->at("cast_time").value_or(0.5);
                std::size_t target = spell->at("target").value_or(SpellTarget::ANY);
                std::vector<SpellEntities> entities;
                if (spell->at("entities").is_array()) {
                    const auto& raw_entities = spell->at("entities").as_array();
                    for (std::size_t i = 0; i < raw_entities->size(); ++i)
                        entities.emplace_back(raw_entities[i].value_or("unkown"));
                }
                spells.emplace_back(name, base, static_cast<EffectType>(dtype),
                    ratios, cd, ctime, static_cast<SpellTarget>(target), mc, entities);
            }
        }
        game.createComponent<Spells>(e, spells);
    });
    game.registerComponent<SpellEntity>("spell_entity",
        [&game](ECS::Entity e, const toml::table& params) {
        std::size_t entity = params["entity"].value_or(0);
        int spellIdx = params["spell"].value_or(0);
        game.createComponent<SpellEntity>(e, entity, spellIdx);
    });
}
