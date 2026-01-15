#pragma once

// Used to calculate the built-in number of a resource
// check_func should be the id to a built-in function such as (object_exists, sprite_exists, sound_exists, etc)
int resources_count(int func_id);