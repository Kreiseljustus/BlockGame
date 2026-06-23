//
// Created by crisel on 6/23/26.
//

#ifndef BLOCKGAME_VECTOR2_H
#define BLOCKGAME_VECTOR2_H


template <typename T>
struct Vector2 {
    T x = 0, y = 0;
};

typedef Vector2<double> Vector2D;
typedef Vector2<float> Vector2F;
typedef Vector2<int> Vector2I;


#endif //BLOCKGAME_VECTOR2_H
