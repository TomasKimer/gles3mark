/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

/**
 * \brief Base class for Texture and RenderBuffer.
 */
class RenderTarget {

protected:

    int width, height;
    int maxSize;
};