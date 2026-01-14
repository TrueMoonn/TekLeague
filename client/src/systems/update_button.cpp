/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** button_status.cpp
*/

#include <ECS/DenseZipper.hpp>
#include <display/components/animation.hpp>
#include <interaction/components/hoverable.hpp>
#include <interaction/components/clickable.hpp>

#include "components/ui/button.hpp"
#include "ui/buttons.hpp"
#include "client_systems.hpp"

void changeButtonState(addon::display::Animation& anim, bStatus status) {
    std::size_t st = static_cast<std::size_t>(status);
    if (anim.curAnim != st) {
        anim.curAnim = st;
        anim.timestamp.delay =
            anim.frameInfos[st].frameDELAY;
    }
}

void buttonStates(Client& client) {
    client.createSystem("button_states", [&client](ECS::Registry&){
        auto& anims = client.getComponent<addon::display::Animation>();
        auto& buttons = client.getComponent<Button>();
        auto& hovers = client.getComponent<addon::intact::Hoverable>();
        auto& clicks = client.getComponent<addon::intact::Clickable>();

        for (auto&& [_, anim, hover, click] :
            ECS::DenseZipper(buttons, anims, hovers, clicks)) {
            if (click.clicked) changeButtonState(anim, bStatus::CLICKED);
            else if (hover.hovered) changeButtonState(anim, bStatus::HOVERED);
            else changeButtonState(anim, bStatus::DEFAULT);
        }
    });
}
