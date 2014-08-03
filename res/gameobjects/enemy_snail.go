enemy
{
    walk_anim = walk
    speed = 0.5
    physics = collision_object
    collision_trigger = trigger
}

collision_object collision_object
{
    physics = res/physics/characters.physics#enemy_snail
}

collision_object trigger
{
    physics = res/physics/characters.physics#enemy_snail_trigger
}

sprite_animation walk
{
    spritesheet = res/spritesheets/enemy.ss
    spriteprefix = snail_walk__
    loop = true
    autostart = true
    fps = 3
}
