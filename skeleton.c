#include "skeleton.h"
#include "player.h"

bool isSkeletonGrounded;
int health = 3;

bool is_dead;

void skeleton_spawn(Level *level, GameManager *manager){
    Entity* skeleton = level_add_entity(level, &skel_desc);
    // TEMP//
    entity_pos_set(skeleton, (Vector){30, 30});
    
    entity_collider_add_rect(skeleton, 15, 31);
    
    SkeletonContext* skeleton_context = entity_context_get(skeleton);
    
    skeleton_context->sprite = game_manager_sprite_load(manager, "enemies/skeleton/walking_0.fxbm");
}

void skel_behavior(Vector* pos) {
    Vector player_pos = entity_pos_get(player);
    PlayerContext* player_context = entity_context_get(player);

    float distance_to_player = pos->x - player_pos.x;

    if(player != NULL) {
        if(player_pos.x > pos->x) pos->x += 0.5;
        if(player_pos.x < pos->x) pos->x -= 0.5;
    }

    // hit logic //
    // hit logic is calculated by checking if the skeleton is in ceratin range of the player
    if (player_context->is_hitting) {
        if (is_player_facing_right && distance_to_player > 0 && distance_to_player <= player_sword_reach) {
            health -= player_context->weapon_damage;
        }
        if (!is_player_facing_right && distance_to_player < 0 && -distance_to_player <= player_sword_reach) {
            health -= player_context->weapon_damage;
        }
    }
}

void skel_update(Entity* self, GameManager* manager, void* context) {
    Vector pos = entity_pos_get(self);
    SkeletonContext* skeleton_context = entity_context_get(self);
    GameContext* game_context = game_manager_game_context_get(manager);

    Level* level = game_manager_current_level_get(manager);

    if((pos.y + 31) >= game_context->ground_hight && skeleton_context->Yvelocity >= 0) {
        pos.y = game_context->ground_hight - 31;
        skeleton_context->Yvelocity = 0;
        isSkeletonGrounded = true;
    } else {
        isSkeletonGrounded = false;
    }

    if(!isSkeletonGrounded) {
        skeleton_context->Yvelocity += 0.5;
        pos.y += skeleton_context->Yvelocity;
    }

    if(pos.y < 5) pos.y = 5;
    if(pos.y > 59) pos.y = 59;

    skel_behavior(&pos);

    entity_pos_set(self, pos);
    
    if(health <= 0){
        level_remove_entity(level, self);
    }
    UNUSED(manager);
    UNUSED(context);
}


void skel_render(Entity* self, GameManager* manager, Canvas* canvas, void* context) {
    SkeletonContext* skel = context;
    Vector pos = entity_pos_get(self);

    canvas_draw_sprite(canvas, skel->sprite, pos.x - 10, pos.y);

    UNUSED(manager);
}

const EntityDescription skel_desc = {
    .start = NULL,
    .stop = NULL,
    .update = skel_update,
    .render = skel_render,
    .collision = NULL,
    .event = NULL,
    .context_size =
        sizeof(SkeletonContext),
};