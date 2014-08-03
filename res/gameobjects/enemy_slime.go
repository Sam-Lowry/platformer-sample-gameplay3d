enemy
{
    walk_anim = walk
    speed = 1.0
    physics = collision_object
    collision_trigger = trigger
}

collision_object collision_object
{
    physics = res/physics/characters.physics#enemy_slime
}

collision_object trigger
{
    physics = res/physics/characters.physics#enemy_slime_trigger
}

sprite_animation walk
{
    spritesheet = res/spritesheets/enemy.ss
    spriteprefix = slime_walk__
    loop = true
    autostart = true
    fps = 3
}
