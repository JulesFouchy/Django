#ifndef color_lab_get_gray
#define color_lab_get_gray

// ::color::get::gray( c )

#include "../../gray/place/place.hpp"
#include "../../gray/akin/lab.hpp"
#include "../../gray/trait/component.hpp"

#include "../category.hpp"

#include "../../_internal/reformat.hpp"
#include "../../generic/trait/scalar.hpp"




 namespace color
  {
   namespace get
    {

     template< typename tag_name, ::color::constant::lab::reference_enum reference_number >
      inline
      typename ::color::trait::component< typename ::color::akin::gray< ::color::category::lab<tag_name, reference_number> >::akin_type >::return_type
      gray( ::color::model< ::color::category::lab<tag_name,reference_number> > const& color_parameter )
       {
        typedef ::color::category::lab< tag_name, reference_number > category_type;

        typedef typename ::color::akin::gray< category_type >::akin_type     akin_category_type;

        typedef ::color::_internal::reformat< akin_category_type, category_type >  reformat_type;

        enum
         {
           lightness_p  = ::color::place::_internal::lightness<category_type>::position_enum
         };

        enum
         {
           gray_p  = ::color::place::_internal::gray<akin_category_type>::position_enum
         };

        return reformat_type::template process<gray_p,lightness_p>( color_parameter.template get<lightness_p>() );
       }

    }
  }

#endif
