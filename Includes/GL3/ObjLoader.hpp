#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <GL3/GLTypes.hpp>
#include <GL3/Vertex.hpp>
#include <string>
#include <vector>
namespace GL3 {

	class ObjLoader
	{
	public:
		static bool LoadFile(const std::string& path, std::vector<float>& vertices, std::vector<unsigned int>& indices, VertexFormat format);
	};

};

#endif //! end of Texture.hpp