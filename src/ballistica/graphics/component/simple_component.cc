// Released under the MIT License. See LICENSE for details.

#include "ballistica/graphics/component/simple_component.h"

namespace ballistica {

void SimpleComponent::WriteConfig() {
  // if we're transparent we don't want to do optimization-based
  // shader swapping (ie: when color is 1).  This is because it can
  // affect draw order, which is important unlike with opaque stuff.
  if (transparent_) {
    if (texture_.exists()) {
      if (colorize_texture_.exists()) {
        assert(flatness_ == 0.0f);            // unimplemented combo
        assert(glow_amount_ == 0.0f);         // unimplemented combo
        assert(shadow_opacity_ == 0.0f);      // unimplemented combo
        assert(!double_sided_);               // unimplemented combo
        assert(!mask_uv2_texture_.exists());  // unimplemented combo
        if (do_colorize_2_) {
          if (mask_texture_.exists()) {
            ConfigForShading(
                ShadingType::
                    kSimpleTextureModulatedTransparentColorized2Masked);
            cmd_buffer_->PutInt(premultiplied_);
            cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                   colorize_color_r_, colorize_color_g_,
                                   colorize_color_b_, colorize_color2_r_,
                                   colorize_color2_g_, colorize_color2_b_);
            cmd_buffer_->PutTexture(texture_);
            cmd_buffer_->PutTexture(colorize_texture_);
            cmd_buffer_->PutTexture(mask_texture_);
          } else {
            ConfigForShading(
                ShadingType::kSimpleTextureModulatedTransparentColorized2);
            cmd_buffer_->PutInt(premultiplied_);
            cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                   colorize_color_r_, colorize_color_g_,
                                   colorize_color_b_, colorize_color2_r_,
                                   colorize_color2_g_, colorize_color2_b_);
            cmd_buffer_->PutTexture(texture_);
            cmd_buffer_->PutTexture(colorize_texture_);
          }
        } else {
          assert(!mask_texture_.exists());  // unimplemented combo
          ConfigForShading(
              ShadingType::kSimpleTextureModulatedTransparentColorized);
          cmd_buffer_->PutInt(premultiplied_);
          cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                 colorize_color_r_, colorize_color_g_,
                                 colorize_color_b_);
          cmd_buffer_->PutTexture(texture_);
          cmd_buffer_->PutTexture(colorize_texture_);
        }
      } else {
        // non-colorized with texture
        if (double_sided_) {
          assert(!mask_texture_.exists());      // unimplemented combo
          assert(flatness_ == 0.0f);            // unimplemented combo
          assert(glow_amount_ == 0.0f);         // unimplemented combo
          assert(shadow_opacity_ == 0.0f);      // unimplemented combo
          assert(!mask_texture_.exists());      // unimplemented combo
          assert(!mask_uv2_texture_.exists());  // unimplemented combo
          ConfigForShading(
              ShadingType::kSimpleTextureModulatedTransparentDoubleSided);
          cmd_buffer_->PutInt(premultiplied_);
          cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_);
          cmd_buffer_->PutTexture(texture_);
        } else {
          if (shadow_opacity_ > 0.0f) {
            assert(!mask_texture_.exists());  // unimplemented combo
            assert(glow_amount_ == 0.0f);     // unimplemented combo
            assert(mask_uv2_texture_.exists());
            if (flatness_ != 0.0f) {
              ConfigForShading(
                  ShadingType::kSimpleTexModulatedTransShadowFlatness);
              cmd_buffer_->PutInt(premultiplied_);
              cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                     shadow_offset_x_, shadow_offset_y_,
                                     shadow_blur_, shadow_opacity_, flatness_);
              cmd_buffer_->PutTexture(texture_);
              cmd_buffer_->PutTexture(mask_uv2_texture_);
            } else {
              ConfigForShading(
                  ShadingType::kSimpleTextureModulatedTransparentShadow);
              cmd_buffer_->PutInt(premultiplied_);
              cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                     shadow_offset_x_, shadow_offset_y_,
                                     shadow_blur_, shadow_opacity_);
              cmd_buffer_->PutTexture(texture_);
              cmd_buffer_->PutTexture(mask_uv2_texture_);
            }
          } else {
            if (glow_amount_ > 0.0f) {
              assert(!mask_texture_.exists());  // unimplemented combo
              assert(flatness_ == 0.0f);        // unimplemented combo
              if (mask_uv2_texture_.exists()) {
                ConfigForShading(
                    ShadingType::kSimpleTextureModulatedTransparentGlowMaskUV2);
                cmd_buffer_->PutInt(premultiplied_);
                cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                       glow_amount_, glow_blur_);
                cmd_buffer_->PutTexture(texture_);
                cmd_buffer_->PutTexture(mask_uv2_texture_);
              } else {
                ConfigForShading(
                    ShadingType::kSimpleTextureModulatedTransparentGlow);
                cmd_buffer_->PutInt(premultiplied_);
                cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                       glow_amount_, glow_blur_);
                cmd_buffer_->PutTexture(texture_);
              }
            } else {
              if (flatness_ != 0.0f) {
                assert(!mask_texture_.exists());  // unimplemented
                ConfigForShading(
                    ShadingType::kSimpleTextureModulatedTransFlatness);
                cmd_buffer_->PutInt(premultiplied_);
                cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                       flatness_);
                cmd_buffer_->PutTexture(texture_);
              } else {
                if (mask_texture_.exists()) {
                  // currently mask functionality requires colorize too, so
                  // just send a black texture for that..
                  ConfigForShading(
                      ShadingType::
                          kSimpleTextureModulatedTransparentColorized2Masked);
                  cmd_buffer_->PutInt(premultiplied_);
                  cmd_buffer_->PutFloats(
                      color_r_, color_g_, color_b_, color_a_, colorize_color_r_,
                      colorize_color_g_, colorize_color_b_, colorize_color2_r_,
                      colorize_color2_g_, colorize_color2_b_);
                  cmd_buffer_->PutTexture(texture_);
                  cmd_buffer_->PutTexture(
                      g_assets->GetTexture(SystemTextureID::kBlack));
                  cmd_buffer_->PutTexture(mask_texture_);
                } else {
                  ConfigForShading(
                      ShadingType::kSimpleTextureModulatedTransparent);
                  cmd_buffer_->PutInt(premultiplied_);
                  cmd_buffer_->PutFloats(color_r_, color_g_, color_b_,
                                         color_a_);
                  cmd_buffer_->PutTexture(texture_);
                }
              }
            }
          }
        }
      }
    } else {
      assert(flatness_ == 0.0f);            // unimplemented combo
      assert(glow_amount_ == 0.0f);         // unimplemented combo
      assert(shadow_opacity_ == 0.0f);      // unimplemented combo
      assert(!colorize_texture_.exists());  // unimplemented combo
      assert(!mask_texture_.exists());      // unimplemented combo
      assert(!mask_uv2_texture_.exists());  // unimplemented combo
      if (double_sided_) {
        ConfigForShading(ShadingType::kSimpleColorTransparentDoubleSided);
        cmd_buffer_->PutInt(premultiplied_);
        cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_);
      } else {
        ConfigForShading(ShadingType::kSimpleColorTransparent);
        cmd_buffer_->PutInt(premultiplied_);
        cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_);
      }
    }
  } else {
    // when we're opaque we can do some shader-swapping optimizations
    // since draw order doesn't matter.
    assert(flatness_ == 0.0f);            // unimplemented combo
    assert(glow_amount_ == 0.0f);         // unimplemented combo
    assert(shadow_opacity_ == 0.0f);      // unimplemented combo
    assert(!double_sided_);               // not implemented
    assert(!mask_uv2_texture_.exists());  // unimplemented combo
    if (texture_.exists()) {
      if (colorize_texture_.exists()) {
        assert(!mask_texture_.exists());  // unimplemented combo
        if (do_colorize_2_) {
          ConfigForShading(ShadingType::kSimpleTextureModulatedColorized2);
          cmd_buffer_->PutFloats(color_r_, color_g_, color_b_,
                                 colorize_color_r_, colorize_color_g_,
                                 colorize_color_b_, colorize_color2_r_,
                                 colorize_color2_g_, colorize_color2_b_);
          cmd_buffer_->PutTexture(texture_);
          cmd_buffer_->PutTexture(colorize_texture_);
        } else {
          ConfigForShading(ShadingType::kSimpleTextureModulatedColorized);
          cmd_buffer_->PutFloats(color_r_, color_g_, color_b_,
                                 colorize_color_r_, colorize_color_g_,
                                 colorize_color_b_);
          cmd_buffer_->PutTexture(texture_);
          cmd_buffer_->PutTexture(colorize_texture_);
        }
      } else {
        assert(!do_colorize_2_);  // unsupported combo
        if (mask_texture_.exists()) {
          // currently mask functionality requires colorize too, so
          // we have to send a black texture along for that..
          ConfigForShading(
              ShadingType::kSimpleTextureModulatedColorized2Masked);
          cmd_buffer_->PutFloats(color_r_, color_g_, color_b_, color_a_,
                                 colorize_color_r_, colorize_color_g_,
                                 colorize_color_b_, colorize_color2_r_,
                                 colorize_color2_g_, colorize_color2_b_);
          cmd_buffer_->PutTexture(texture_);
          cmd_buffer_->PutTexture(
              g_assets->GetTexture(SystemTextureID::kBlack));
          cmd_buffer_->PutTexture(mask_texture_);
        } else {
          // if no color was provided we can do a super-cheap version
          if (!have_color_) {
            ConfigForShading(ShadingType::kSimpleTexture);
            cmd_buffer_->PutTexture(texture_);
          } else {
            ConfigForShading(ShadingType::kSimpleTextureModulated);
            cmd_buffer_->PutFloats(color_r_, color_g_, color_b_);
            cmd_buffer_->PutTexture(texture_);
          }
        }
      }
    } else {
      assert(!mask_texture_.exists());      // unimplemented combo
      assert(!colorize_texture_.exists());  // unsupported here
      ConfigForShading(ShadingType::kSimpleColor);
      cmd_buffer_->PutFloats(color_r_, color_g_, color_b_);
    }
  }
}
}  // namespace ballistica
