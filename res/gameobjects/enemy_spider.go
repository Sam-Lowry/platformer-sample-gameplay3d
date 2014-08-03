enemy
{
    walk_anim = walk
    speed = 1.5
    physics = collision_object
    collision_trigger = trigger
}

collision_object collision_object
{
    physics = res/physics/characters.physics#enemy_spider
}

collision_object trigger
{
    physics = res/physics/characters.physics#enemy_spider_trigger
}

sprite_animation walk
{
    spritesheet = res/spritesheets/enemy.ss
    spriteprefix = spider_walk__
    loop = true
    autostart = true
    fps = 8
}
