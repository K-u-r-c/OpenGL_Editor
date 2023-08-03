#ifndef INC_3D_ENGINE_PARTICLE_H
#define INC_3D_ENGINE_PARTICLE_H

#include "core.h"

namespace feather {


class particle {
public:

private:

protected:
  vector3 position;
  vector3 velocity;
  vector3 acceleration;
  real damping;
  real inverseMass;
};


}

#endif //INC_3D_ENGINE_PARTICLE_H
