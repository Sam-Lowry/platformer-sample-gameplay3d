player
{
    idle_anim = idle
    walk_anim = walk
    duck_anim = duck
    cower_anim = cower
    jump_anim = jump
    climb_anim = climb
    speed = 3.0
    jump_height = 0.7
    normal_physics = character_normal
    ducking_physics = character_ducking
}

player_input
{
}

player_audio
{
    jump_sound = res/audio/player.audio#jump
}

player_hand_of_god
{
    level_boundary_scale = 2, 1.25
}

collision_object character_normal
{
    physics = res/physics/characters.physics#player_normal
}

collision_object character_ducking
{
    physics = res/physics/characters.physics#player_ducking
}

sprite_animation walk
{
    spritesheet = res/spritesheets/player.ss
    spriteprefix = player_walk__
    loop = true
}

sprite_animation climb : walk
{
    spriteprefix = player_climb__
    fps = 5
}

sprite_animation duck
{
    spritesheet = res/spritesheets/player.ss
    spriteprefix = player_duck
}

sprite_animation idle
{
    spritesheet = res/spritesheets/player.ss
    sprite = player_stand
    sprite = player_front
    fps = 0.25
    loop = true
    autostart = true
}

sprite_animation cower : duck
{
    spriteprefix = player_hurt
}

sprite_animation jump
{
    spritesheet = res/spritesheets/player.ss
    sprite = player_jump
    sprite = player_hurt
    fps = 3
}
