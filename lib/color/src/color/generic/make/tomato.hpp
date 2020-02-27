#ifndef  color_generic_make_tomato
#define color_generic_make_tomato

// ::color::make::tomato( c )

// TODO #include "model.hpp"
#include "../constant/tomato.hpp"


 namespace color
  {
   namespace constant
    {

     template< typename category_name >
      struct make< ::color::constant::tomato_t, category_name >
       {
        typedef category_name                         category_type;
        
        typedef ::color::model<category_type>            model_type;
        typedef ::color::rgb<std::uint8_t>                 rgb_type;
        typedef ::color::constant::tomato_t           constant_type;

        typedef typename ::color::trait::container<category_type>::output_type       container_output_type;

        
        inline static void process( container_output_type & container )
         {
          static model_type  s_model{ rgb_type{ 0xFF, 0x63, 0x47 } };
          container = s_model.container();
         }
       };

    }
  }

#endif
