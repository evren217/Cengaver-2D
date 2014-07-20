#pragma once

#define CLEAN(obj) if(obj){delete obj; obj=0;}

#define CONTAINS(x,v) (x&v)==v