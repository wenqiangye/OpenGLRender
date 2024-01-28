/**
 * @author [yesky]
 * @email [a316606581@gmail.com]
 * @create date 2024-01-10 15:32:57
 * @modify date 2024-01-10 15:32:57
 * @desc [description]
 */
#pragma once

#include <memory>
#include <string>
#include <vector>
namespace YeskyGL {
enum WrapMode {
  Wrap_REPEAT,
  Wrap_MIRRORED_REPEAT,
  Wrap_CLAMP_TO_EDGE,
  Wrap_CLAMP_TO_BORDER,
};

enum FilterMode {
  Filter_NEAREST,
  Filter_LINEAR,
  Filter_NEAREST_MIPMAP_NEAREST,
  Filter_LINEAR_MIPMAP_NEAREST,
  Filter_NEAREST_MIPMAP_LINEAR,
  Filter_LINEAR_MIPMAP_LINEAR,
};

enum CubeMapFace {
  TEXTURE_CUBE_MAP_POSITIVE_X = 0,
  TEXTURE_CUBE_MAP_NEGATIVE_X = 1,
  TEXTURE_CUBE_MAP_POSITIVE_Y = 2,
  TEXTURE_CUBE_MAP_NEGATIVE_Y = 3,
  TEXTURE_CUBE_MAP_POSITIVE_Z = 4,
  TEXTURE_CUBE_MAP_NEGATIVE_Z = 5,
};

enum BorderColor {
  Border_BLACK = 0,
  Border_WHITE,
};

struct SamplerDesc {
  FilterMode filterMin = Filter_NEAREST;
  FilterMode filterMag = Filter_NEAREST;

  WrapMode wrapS = Wrap_CLAMP_TO_EDGE;
  WrapMode wrapT = Wrap_CLAMP_TO_EDGE;
  WrapMode wrapR = Wrap_CLAMP_TO_EDGE;

  BorderColor borderColor = Border_BLACK;
};

enum TextureType {
  TextureType_2D,
  TextureType_CUBE,
};

enum TextureFormat {
  TextureFormat_RGBA8 = 0,
  TextureFormat_FLOAT32 = 1,
};

enum TextureUsage {
  TextureUsage_Sampler = 1 << 0,
  TextureUsage_UploadDate = 1 << 1,
  TextureUsage_AttachmentColor = 1 << 2,
  TextureUsage_AttachmentDepth = 1 << 3,
  TextureUsage_RendererOutput = 1 << 4,
};

struct TextureDesc {
  int width = 0;
  int height = 0;

  TextureType type = TextureType_2D;
  TextureFormat format = TextureFormat_RGBA8;
  uint32_t usage = TextureUsage_Sampler;
  bool useMipmaps = false;
  bool multiSample = false;
  std::string tag;
};

class Texture : public TextureDesc {
 public:
  virtual ~Texture() = default;
  inline uint32_t getLevelWidth(uint32_t level) {
    return std::max(1, width >> level);
  }
  inline uint32_t getLevelHeight(uint32_t level) {
    return std::max(1, height >> level);
  }

  virtual int getID() const = 0;
  virtual void setSamplerDesc(SamplerDesc &sampler){};
  virtual void initImageData(){};
  virtual void setImageData(){};
  virtual void dumpImage(const char *path, uint32_t layer, uint32_t level) = 0;
};
}  // namespace YeskyGL