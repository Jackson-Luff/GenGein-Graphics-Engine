#pragma once
#include <string>
#include <fstream>
#include <streambuf>

#include <glm\fwd.hpp>

#define TINYOBJ_LOADER_OPT_IMPLEMENTATION
#include "tinyOBJ\tinyobj_loader_opt.h"

using namespace tinyobj_opt;

namespace tinyObj
{

	glm::vec3 CalcNormal(glm::vec3 a_v0, glm::vec3 a_v1, glm::vec3 a_v2)
	{
		glm::vec3 v10, v20, n;

		for (int i = 0; i < 3; i++) {
			v10[i] = a_v1[i] - a_v0[i];
			v20[i] = a_v2[i] - a_v0[i];
		}

		n.y = v20.z * v10.x - v20.x * v10.z;
		n.z = v20.x * v10.y - v20.y * v10.x;
		n.x = v20.y * v10.z - v20.z * v10.y;

		float len2 = (n.x * n.x) + (n.y * n.y) + (n.z * n.z);
		if (len2 > 0.0f) {
			float len = sqrtf(len2);

			n /= len; // was : n.x&y /= len
		}

		return n;
	}

	const char *mmap_file(size_t *len, const char* filename)
	{
		(*len) = 0;
#ifdef _WIN64
		HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		assert(file != INVALID_HANDLE_VALUE);

		HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
		assert(fileMapping != INVALID_HANDLE_VALUE);

		LPVOID fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
		auto fileMapViewChar = (const char*)fileMapView;
		assert(fileMapView != NULL);

		return fileMapViewChar;
#else

		FILE* f = fopen(filename, "r");
		fseek(f, 0, SEEK_END);
		long fileSize = ftell(f);
		fclose(f);

		struct stat sb;
		char *p;
		int fd;

		fd = open(filename, O_RDONLY);
		if (fd == -1) {
			perror("open");
			return NULL;
		}

		if (fstat(fd, &sb) == -1) {
			perror("fstat");
			return NULL;
		}

		if (!S_ISREG(sb.st_mode)) {
			fprintf(stderr, "%s is not a file\n", "lineitem.tbl");
			return NULL;
		}

		p = (char*)mmap(0, fileSize, PROT_READ, MAP_SHARED, fd, 0);

		if (p == MAP_FAILED) {
			perror("mmap");
			return NULL;
		}

		if (close(fd) == -1) {
			perror("close");
			return NULL;
		}

		(*len) = fileSize;

		return p;

#endif
	}

	bool gz_load(std::vector<char>* buf, const char* filename)
	{
#ifdef ENABLE_ZLIB
		gzFile file;
		file = gzopen(filename, "r");
		if (!file) {
			fprintf(stderr, "gzopen of '%s' failed: %s.\n", filename,
				strerror(errno));
			exit(EXIT_FAILURE);
			return false;
		}
		while (1) {
			int err;
			int bytes_read;
			unsigned char buffer[1024];
			bytes_read = gzread(file, buffer, 1024);
			buf->insert(buf->end(), buffer, buffer + 1024);
			//printf ("%s", buffer);
			if (bytes_read < 1024) {
				if (gzeof(file)) {
					break;
				}
				else {
					const char * error_string;
					error_string = gzerror(file, &err);
					if (err) {
						fprintf(stderr, "Error: %s.\n", error_string);
						exit(EXIT_FAILURE);
						return false;
					}
				}
			}
		}
		gzclose(file);
		return true;
#else
		return false;
#endif
	}

#ifdef ENABLE_ZSTD
	static off_t fsize_X(const char *filename)
	{
		struct stat st;
		if (stat(filename, &st) == 0) return st.st_size;
		/* error */
		printf("stat: %s : %s \n", filename, strerror(errno));
		exit(1);
	}

	static FILE* fopen_X(const char *filename, const char *instruction)
	{
		FILE* const inFile = fopen(filename, instruction);
		if (inFile) return inFile;
		/* error */
		printf("fopen: %s : %s \n", filename, strerror(errno));
		exit(2);
	}

	static void* malloc_X(size_t size)
	{
		void* const buff = malloc(size);
		if (buff) return buff;
		/* error */
		printf("malloc: %s \n", strerror(errno));
		exit(3);
	}
#endif

	bool zstd_load(std::vector<char>* buf, const char* filename)
	{
#ifdef ENABLE_ZSTD
		off_t const buffSize = fsize_X(filename);
		FILE* const inFile = fopen_X(filename, "rb");
		void* const buffer = malloc_X(buffSize);
		size_t const readSize = fread(buffer, 1, buffSize, inFile);
		if (readSize != (size_t)buffSize) {
			printf("fread: %s : %s \n", filename, strerror(errno));
			exit(4);
		}
		fclose(inFile);

		unsigned long long const rSize = ZSTD_getDecompressedSize(buffer, buffSize);
		if (rSize == 0) {
			printf("%s : original size unknown \n", filename);
			exit(5);
		}

		buf->resize(rSize);

		size_t const dSize = ZSTD_decompress(buf->data(), rSize, buffer, buffSize);

		if (dSize != rSize) {
			printf("error decoding %s : %s \n", filename, ZSTD_getErrorName(dSize));
			exit(7);
		}

		free(buffer);

		return true;
#else
		return false;
#endif
	}

	const char* get_file_data(size_t *len, const char* filename)
	{

		const char *ext = strrchr(filename, '.');

		size_t data_len = 0;
		const char* data = nullptr;

		if (strcmp(ext, ".gz") == 0) {
			// gzipped data.

			std::vector<char> buf;
			bool ret = gz_load(&buf, filename);

			if (ret) {
				char *p = static_cast<char*>(malloc(buf.size() + 1));  // @fixme { implement deleter }
				memcpy(p, &buf.at(0), buf.size());
				p[buf.size()] = '\0';
				data = p;
				data_len = buf.size();
			}

		}
		else if (strcmp(ext, ".zst") == 0) {
			printf("zstd\n");
			// Zstandard data.

			std::vector<char> buf;
			bool ret = zstd_load(&buf, filename);

			if (ret) {
				char *p = static_cast<char*>(malloc(buf.size() + 1));  // @fixme { implement deleter }
				memcpy(p, &buf.at(0), buf.size());
				p[buf.size()] = '\0';
				data = p;
				data_len = buf.size();
			}
		}
		else {

			data = mmap_file(&data_len, filename);
		}

		(*len) = data_len;
		return data;
	}


	bool LoadObjAndConvert(const char* filename, int num_threads, bool verbose)
	{
		struct OpenGLInfo
		{
			glm::vec3 p; // position
			glm::vec3 n; // normal
			glm::vec3 c; // colour
		};

		tinyobj_opt::attrib_t attrib;
		std::vector<tinyobj_opt::shape_t> shapes;
		std::vector<tinyobj_opt::material_t> materials;

		auto load_t_begin = std::chrono::high_resolution_clock::now();
		size_t data_len = 0;
		const char* data = get_file_data(&data_len, filename);
		if (data == nullptr) {
			printf("failed to load file\n");
			exit(-1);
			return false;
		}
		auto load_t_end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> load_ms = load_t_end - load_t_begin;
		if (verbose) {
			std::cout << "filesize: " << data_len << std::endl;
			std::cout << "load time: " << load_ms.count() << " [msecs]" << std::endl;
		}

		tinyobj_opt::LoadOption option;
		option.req_num_threads = num_threads;
		option.verbose = verbose;
		bool ret = parseObj(&attrib, &shapes, &materials, data, data_len, option);

		std::vector<OpenGLInfo> vb; // pos(3float), normal(3float), color(3float)
		size_t face_offset = 0;

		for (size_t v = 0; v < attrib.face_num_verts.size(); v++)
		{
			assert(attrib.face_num_verts[v] % 3 == 0); // assume all triangle face.
			for (size_t f = 0; f < attrib.face_num_verts[v] / 3; f++) {
				tinyobj_opt::index_t idx0 = attrib.indices[face_offset + 3 * f + 0];
				tinyobj_opt::index_t idx1 = attrib.indices[face_offset + 3 * f + 1];
				tinyobj_opt::index_t idx2 = attrib.indices[face_offset + 3 * f + 2];

				OpenGLInfo info[3];
				for (int k = 0; k < 3; k++) {
					int vf0 = idx0.vertex_index;
					int vf1 = idx1.vertex_index;
					int vf2 = idx2.vertex_index;

					assert(vf0 >= 0 || vf1 >= 0 || vf2 >= 0);

					info[k].p.x = attrib.vertices[3 * vf0 + k];
					info[k].p.y = attrib.vertices[3 * vf1 + k];
					info[k].p.z = attrib.vertices[3 * vf2 + k];
				}

				// Normal calculation
				if (attrib.normals.size() > 0) {
					int nf0 = idx0.normal_index;
					int nf1 = idx1.normal_index;
					int nf2 = idx2.normal_index;

					if (nf0 >= 0 && nf1 >= 0 && nf2 >= 0) {
						assert(3 * nf0 + 2 < attrib.normals.size());
						assert(3 * nf1 + 2 < attrib.normals.size());
						assert(3 * nf2 + 2 < attrib.normals.size());
						for (int k = 0; k < 3; k++) {
							info[k].n.x = attrib.normals[3 * nf0 + k];
							info[k].n.y = attrib.normals[3 * nf1 + k];
							info[k].n.z = attrib.normals[3 * nf2 + k];
						}
					}
					else {
						// compute geometric normal
						info[0].n = CalcNormal(info[0].p, info[1].p, info[2].p);
						info[1].n.y = info[0].n.x; info[1].n.y = info[1].n.x; info[2].n.y = info[2].n.x;
						info[2].n.z = info[0].n.x; info[1].n.z = info[1].n.x; info[2].n.z = info[2].n.x;
					}
				}
				else {
					// compute geometric normal
					info[0].n = CalcNormal(info[0].p, info[1].p, info[2].p);
					info[0].n.y = info[0].n.x; info[1].n.y = info[1].n.x; info[2].n.y = info[2].n.x;
					info[0].n.z = info[0].n.x; info[1].n.z = info[1].n.x; info[2].n.z = info[2].n.x;
				}

				for (int k = 0; k < 3; k++)
				{
					// Use normal as color.
					info[k].c = glm::vec3(info[k].n.x, info[k].n.y, info[k].n.z);
					float len2 = info[k].c[0] * info[k].c[0] + info[k].c[1] * info[k].c[1] + info[k].c[2] * info[k].c[2];
					if (len2 > 1.0e-6f) {
						float len = sqrtf(len2);

						info[0].c /= len;
						info[1].c /= len;
						info[2].c /= len;
					}

					info[0].c = (info[0].c * 0.5f) + 0.5f;
					info[1].c = (info[1].c * 0.5f) + 0.5f;
					info[2].c = (info[2].c * 0.5f) + 0.5f;

					vb.push_back(info[0]);
					vb.push_back(info[1]);
					vb.push_back(info[2]);

				}
			}
			face_offset += attrib.face_num_verts[v];
		}

		//o.vb = 0;
		//o.numTriangles = 0;
		//if (vb.size() > 0) 
		//{
		//	glGenBuffers(1, &o.vb);
		//	glBindBuffer(GL_ARRAY_BUFFER, o.vb);
		//	glBufferData(GL_ARRAY_BUFFER, vb.size() * sizeof(float), &vb.at(0), GL_STATIC_DRAW);
		//	o.numTriangles = vb.size() / 9 / 3;
		//}

		return true;
	}
}