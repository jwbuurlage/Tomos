#pragma once

#include <vector>

#include "common.hpp"

namespace tomo {

/**
 * An object that represents the measurements for a given geometry (and
 * projector).
 *
 * \tparam D the dimension of the volume (and thus the reconstruction problem).
 * \tparam T the scalar type to use
 */
template <dimension D, typename T>
class projections {
  public:
    using value_type = T;

    /** Construct default-initialized projections for a geometry. */
    projections(const geometry::base<D, T>& geometry) : geometry_(geometry) {
        data_.resize(geometry.lines());
    }

    /**
     * Obtain a reference to the i-th measurement, corresponding to the i-th
     * line.
     * \param i the index of the line
     */
    T& operator[](int i) { return data_[i]; }
    const T& operator[](int i) const { return data_[i]; }

    /**
     * Obtain a reference to the underlying projection data.
     */
    const std::vector<T>& data() const { return data_; }
    std::vector<T>& mutable_data() { return data_; }

    /** Clear the projection stack. Sets each measurement to zero. */
    void clear() { std::fill(data_.begin(), data_.end(), 0); }

    ///** Get a projection as an image */
    image<D - 1, T> get_projection(int idx) const {
        auto offset = compute_offset_(idx);
        auto shape = geometry_.projection_shape(idx);
        auto img = image<D - 1, T>(volume<D - 1, T>(shape));
        for (int i = 0; i < math::reduce<D - 1>(shape); ++i) {
            img[i] = data_[offset + i];
        }
        return img;
    }

    /** Set a projection using an image */
    void set_projection(int idx, const image<D - 1, T>& img) {
        auto offset = compute_offset_(idx);
        auto pixels = math::reduce<D - 1>(geometry_.projection_shape(idx));
        for (int i = 0; i < pixels; ++i) {
            data_[offset + i] = img[i];
        }
    }

    int offset(int idx) const {
        return compute_offset_(idx);
    }

  private:
    int compute_offset_(int idx) const {
        // FIXME cache this
        auto offset = 0;
        for (int i = 0; i < idx; ++i) {
            offset += math::reduce<D - 1>(geometry_.projection_shape(i));
        }
        return offset;
    }

    const geometry::base<D, T>& geometry_;
    int projection_count_;
    std::vector<T> data_;
};

} // namespace tomo
