//*****************************************************************************
// Copyright 2017-2021 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

#pragma once

#include <cstddef>

#include "ngraph/util.hpp"

namespace ngraph
{
    namespace runtime
    {
        /// \brief Allocates a block of memory on the specified alignment. The actual size of the
        /// allocated memory is larger than the requested size by the alignment, so allocating 1
        /// byte
        /// on 64 byte alignment will allocate 65 bytes.
        class NGRAPH_API AlignedBuffer
        {
        public:
            // Allocator objects and the allocation interfaces are owned by the
            // creators of AlignedBuffers. They need to ensure that the lifetime of
            // allocator exceeds the lifetime of this AlignedBuffer.
            AlignedBuffer(size_t byte_size, size_t alignment = 64);

            AlignedBuffer();
            ~AlignedBuffer();

            AlignedBuffer(AlignedBuffer&& other);
            AlignedBuffer& operator=(AlignedBuffer&& other);

            size_t size() const { return m_byte_size; }
            void* get_ptr(size_t offset) const { return m_aligned_buffer + offset; }
            void* get_ptr() { return m_aligned_buffer; }
            const void* get_ptr() const { return m_aligned_buffer; }
            template <typename T>
            T* get_ptr()
            {
                return reinterpret_cast<T*>(m_aligned_buffer);
            }
            template <typename T>
            const T* get_ptr() const
            {
                return reinterpret_cast<const T*>(m_aligned_buffer);
            }

            template <typename T>
            explicit operator T*()
            {
                return get_ptr<T>();
            }

        private:
            AlignedBuffer(const AlignedBuffer&) = delete;
            AlignedBuffer& operator=(const AlignedBuffer&) = delete;

        protected:
            char* m_allocated_buffer;
            char* m_aligned_buffer;
            size_t m_byte_size;
        };
    }
    template <>
    class NGRAPH_API AttributeAdapter<std::shared_ptr<runtime::AlignedBuffer>>
        : public DirectValueAccessor<std::shared_ptr<runtime::AlignedBuffer>>
    {
    public:
        AttributeAdapter(std::shared_ptr<runtime::AlignedBuffer>& value);

        static constexpr DiscreteTypeInfo type_info{
            "AttributeAdapter<std::shared_ptr<runtime::AlignedBuffer>>", 0};
        const DiscreteTypeInfo& get_type_info() const override { return type_info; }
    };
}
