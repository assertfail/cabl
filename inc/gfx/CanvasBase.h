/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "Canvas.h"
#include <array>
#include <bitset>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

/**
  \class CanvasBase
  \brief The canvas base class

*/
template <unsigned W, unsigned H, unsigned SIZE = W* H * 3, unsigned NCHUNKS = 1>
class CanvasBase : public Canvas
{

public:
  CanvasBase()
  {
    setDirty();
  }

  unsigned width() const noexcept override
  {
    return W;
  }

  unsigned height() const noexcept override
  {
    return H;
  }

  unsigned canvasWidthInBytes() const noexcept override
  {
    return H > 0 ? SIZE / H : 0;
  }

  unsigned numberOfChunks() const noexcept override
  {
    return NCHUNKS;
  }

  const uint8_t* buffer() override
  {
    return m_data.data();
  }

  unsigned bufferSize() const override
  {
    return SIZE;
  }

  const uint8_t* data() const override
  {
    return m_data.data();
  }

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup GDisplay
   * @{
   */

  //! Sets all of the dirty flags to true
  void setDirty() override
  {
    m_chunkDirtyFlags.set();
  }

  //! Is any of the display chunks dirty?
  /*!
     \return  TRUE if any of the display chunks is dirty, FALSE otherwise
  */
  bool dirty() const override
  {
    return m_chunkDirtyFlags.any();
  }

  //! Is a specific display chunk dirty?
  /*!
     \param chunk_   The display chunk index
     \return         TRUE if the selected display chunk is dirty, FALSE otherwise
     */
  bool dirtyChunk(uint8_t chunk_) const override
  {
    if (chunk_ >= NCHUNKS || NCHUNKS == 0)
    {
      return false;
    }
    return m_chunkDirtyFlags[chunk_];
  }

  void setDirtyChunk(uint16_t yStart_) const override
  {
    unsigned constexpr chunkHeight = H / NCHUNKS;
    if (chunkHeight == 0 || NCHUNKS == 0)
    {
      return;
    }
    if (yStart_ < H)
    {
      unsigned chunk = std::min(static_cast<unsigned>(yStart_ / chunkHeight), NCHUNKS - 1);
      m_chunkDirtyFlags[chunk] = true;
    }
  }

  //! Reset the global dirty flag and the chunk-specific dirty flags
  void resetDirtyFlags() const override
  {
    m_chunkDirtyFlags.reset();
  }

  /** @} */ // End of group Access

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Utility Utility/debug functions
   * @ingroup GDisplay
   * @{
   */


  /** @} */ // End of group Utility

protected:
  uint8_t* data() override
  {
    return m_data.data();
  }

private:
  friend class py::CanvasHelper;

  std::array<uint8_t, SIZE> m_data{};               //!< The raw Canvas data
  mutable std::bitset<NCHUNKS> m_chunkDirtyFlags{}; //!< Chunk-specific dirty flags
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
