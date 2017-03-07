/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_sp_pic_types_h
#define flecsi_sp_pic_types_h

#include <flecsi/topology/mesh_topology.h>

#include "flecsi-sp/geometry/point.h"
#include "flecsi-sp/pic/entity_types.h"

///
// \file pic_types.h
// \authors bergen bird
// \date Initial file creation: Oct 19, 2016
///

namespace flecsi {
namespace sp {

///
// \class pic_types_t pic_types.h
// \brief pic_types_t provides...
///
struct pic_types_t
{
  //--------------------------------------------------------------------------//
  // Define local traits to satisfy mesh_topology requirements.
  //--------------------------------------------------------------------------//

  /// The dimension of the mesh
  static constexpr size_t num_dimensions =
    pic_config_t::num_dimensions;

  /// The number of domains
  static constexpr size_t num_domains =
    pic_config_t::num_domains;

  //--------------------------------------------------------------------------//
  // Define basic types.
  //--------------------------------------------------------------------------//

  /// Mesh vertex type
  using vertex_t = pic_vertex_t;

  /// Mesh cell type
  using cell_t = pic_cell_t;

  /// Convenience type
  template<size_t D>
  using domain_ = topology::domain_<D>;

  ///
  // Definitions of burton mesh entities and their domain.
  // clang-format off
  ///
  using entity_types =
      std::tuple<
        std::pair<domain_<0>, vertex_t>,
        std::pair<domain_<0>, cell_t>
      >;

  ///
  // Connectivities are adjacencies of entities within a single domain.
  ///
  using connectivities =
    std::tuple<
      std::tuple<domain_<0>, vertex_t, cell_t>,
      std::tuple<domain_<0>, cell_t, vertex_t>
    >;

  ///
  // Bindings are adjacencies of entities across two domains.
  ///
  using bindings = std::tuple<>;

  //-------------------------------------------------------------------------//
  //
  //-------------------------------------------------------------------------//

  ///
  // \tparam M The topological domain.
  // \tparam D The topological dimension for which to create an entity.
  ///
  template<
    size_t M,
    size_t D
  >
  static topology::mesh_entity_base_t<num_domains> *
  create_entity(
    topology::mesh_topology_base_t * mesh,
    size_t num_vertices
  )
  {
    assert(false && "this specialization should not create entity types");
  } // create_entity

}; // class pic_types_t

} // namespace sp
} // namespace flecsi

#endif // flecsi_sp_pic_types_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/