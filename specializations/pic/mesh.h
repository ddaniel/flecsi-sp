/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef felcsisp_pic_mesh_h
#define felcsisp_pic_mesh_h

#include "flecsi-sp/geometry/point.h"
#include "flecsi-sp/pic/types.h"

///
// \file pic_mesh.h
// \authors bergen
// \date Initial file creation: Oct 19, 2016
///

namespace flecsi {
  namespace sp {

    ///
    // Use this namespace to expose enumerations and types.
    ///
    namespace pic {

      enum pic_index_spaces_t : size_t {
        vertices,
        cells
      }; // enum pic_index_spaces_t

      enum pic_entity_index_spaces_t : size_t {
        interior,
        boundary
      }; // enum pic_entity_index_spaces_t


      ///
      // \class mesh_t pic_mesh.h
      // \brief mesh_t provides...
      ///
      class mesh_t
        : public topology::mesh_topology_t<types>
      {
        public:

          using base_t = topology::mesh_topology_t<types>;

          static constexpr size_t dimension = pic_config_t::num_dimensions;

          using vertex_t = types::vertex_t;
          using cell_t = types::cell_t;

          using point_t = pic_config_t::point_t;

          /// Default constructor
          mesh_t()
            : base_t() {}

          /// Copy constructor (disabled)
          mesh_t(const mesh_t &) = delete;

          /// Assignment operator (disabled)
          mesh_t & operator = (const mesh_t &) = delete;

          /// Destructor
          ~mesh_t() {}

          ///
          // Add a vertex to the mesh topology.
          ///
          vertex_t *
            make_vertex(
                const point_t & pos,
                entity_type_t type
                )
            {
              auto v = base_t::make<vertex_t>(*this, pos, type);
              base_t::add_entity<0, 0>(v);
              return v;
            } // make_vertex

          ///
          // Add a cell to the mesh topology
          ///
          cell_t *
            make_cell(
                const std::initializer_list<vertex_t *> & vertices,
                entity_type_t type
                )
            {
              auto c = base_t::make<cell_t>(*this, type);
              base_t::add_entity<dimension, 0>(c);
              base_t::init_entity<0, dimension, 0>(c, vertices);
              return c;
            } // make_cell

          ///
          //
          ///
          size_t
            indices(
                size_t index_space_id
                )
            const
            override
            {
              switch(index_space_id) {
                case pic::vertices:
                  return base_t::num_entities(0);
                case pic::cells:
                  return base_t::num_entities(dimension);
                default:
                  assert(false && "unknown index space");
              } // switch
            } // indices

          ///
          //
          ///
          auto
            vertices()
            {
              return base_t::entities<0, 0>();
            } // vertices

          ///
          //
          ///
          template<
            typename E
            >
            auto
            vertices(
                E* e
                )
            {
              return base_t::entities<0, 0>(e);
            } // vertices

          template<class E>
            auto vertices(topology::domain_entity<0, E> & e){
              return vertices(e.entity());
            }
          /*
             template<
             typename E
             >
             auto
             vertices(
             E& e // TODO: Check with Ben if I should have needed to add this reference version
             )
             {
             return base_t::entities<0, 0>(e);
             } // vertices
             */



          ///
          //
          ///
          auto
            cells()
            {
              return base_t::entities<dimension, 0>();
            } // cells

          ///
          //
          ///
          template<
            typename E
            >
            auto
            cells(E * e)
            {
              return base_t::entities<dimension, 0>(e);
            } // cells

          // this is needed for nested iteration (i.e loop over cells then the
          // associated vertices)
          template<class E>
            auto cells(topology::domain_entity<0, E> & e){
              return cells(e.entity());
            }

          auto
            cells(
                size_t is
                )
            {
              switch(is) {
                case pic::interior:
                  return interior_cells_;
                case pic::boundary:
                  return boundary_cells_;
                default:
                  assert(false && "unknown index space");
              } // switch
            } // cells

          auto
            vertices(
                size_t is
                )
            {
              switch(is) {
                case pic::interior:
                  return interior_vertices_;
                case pic::boundary:
                  return boundary_vertices_;
                default:
                  assert(false && "unknown index space");
              } // switch
            } // cells

          /*
             auto
             cells(
             flecsi::topology::domain_entity<0ul, flecsi::sp::pic_vertex_t>& e
             )
             {
             return base_t::entities<dimension, 0>(e);
             } // cells
             */

          // Have the index space map from particle_array_index to the index space
          using simple_t = topology::simple_entry<size_t>;

          void init();

        private:

          topology::index_space<simple_t, false, true, false> particles_is;

          ///
          // Predicate function to create index space for accessing
          // domain boundary cells.
          ///
          static bool is_domain_boundary(
              entity_type_t e
              )
          {
            return e == entity_type_t::domain_boundary;
          }

          template<class E>
            static
            bool
            is_domain_boundary(
                //cell_t * c
                E* c
                )
            {
              std::cout << "Tyoe " << typeid(E).name() << std::endl;
              std::cout << "NOT specialized" << std::endl;
              assert(0); // Something went wrong if we got here..
            } // is_interior

          ///
          // Predicate function to create index space for accessing
          // interior cells.
          ///
          template<class E>
            static
            bool
            is_interior(
                //cell_t * c
                E* c
                )
            {
              std::cout << "Tyoe " << typeid(E).name() << std::endl;
              std::cout << "NOT specialized" << std::endl;
              assert(0); // Something went wrong if we got here..
            } // is_interior

          topology::index_space<
            topology::domain_entity<0, cell_t>, false, true, false> interior_cells_;

          topology::index_space<
            topology::domain_entity<0, cell_t>, false, true, false> boundary_cells_;

          topology::index_space<
            topology::domain_entity<0, vertex_t>, false, true, false> interior_vertices_;

          topology::index_space<
            topology::domain_entity<0, vertex_t>, false, true, false> boundary_vertices_;

      }; // class mesh_t

      using cell_t = types::cell_t;
      template<>
        bool
        mesh_t::is_interior<cell_t>(
            cell_t* c
            )
        {
          return !is_domain_boundary(c->type());
        }

      using vertex_t = types::vertex_t;
      template<>
        bool
        mesh_t::is_interior<vertex_t>(
            vertex_t* v
            )
        {
          return !is_domain_boundary(v->type());
        }

      using cell_t = types::cell_t;
      template<>
        bool
        mesh_t::is_domain_boundary<cell_t>(
            cell_t* c
            )
        {
          return is_domain_boundary(c->type());
        }

      /*
      using vertex_t = types::vertex_t;
      template<>
      bool
      mesh_t::is_interior<vertex_t>(
      vertex_t* c
      )
      {
        std::cout << "vertex special specialized" << std::endl;
        return !is_domain_boundary(c->type());
      }
      */

      // I had to move this outside as it uses the specialized is_interior
      ///
      // Initialize the mesh.
      ///
      void
        mesh_t::init()
        {
          // Initialize domain 0 of the mesh topology.
          base_t::init<0>();

          // Use a predicate function to create the interior cells
          // index space
          interior_cells_ =
            base_t::entities<dimension, 0>().filter(is_interior<cell_t>);

          // Use a predicate function to create the domain boundary cells
          // index space
          boundary_cells_ =
            base_t::entities<dimension, 0>().filter(is_domain_boundary<cell_t>);

          // TODO: Check these indices
          interior_vertices_ =
            base_t::entities<0, 0>().filter(is_interior<vertex_t>);

          /*
             boundary_vertices_ =
             base_t::entities<1?, 0>().filter(is_domain_boundary<vertex_t>);
             */

          //register_data(m, hydro, pressure, double, global, 1);

        } // init

    } // namespace pic
  } // namespace sp
} // namespace flecsi

#endif // felcsisp_pic_mesh_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
