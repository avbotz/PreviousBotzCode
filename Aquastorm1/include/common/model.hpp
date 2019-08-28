#ifndef COMMON_MODEL_HPP 
#define COMMON_MODEL_HPP 

struct Model 
{
	float x, y, z;
	Model(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z) {}
};

inline std::ostream & operator<<(std::ostream &s, const Model &model) {
	s << "m " + std::to_string(model.x) + " " + std::to_string(model.y) + " " + std::to_string(model.z);
	return s;
}

#endif
