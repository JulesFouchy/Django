#ifndef  color_generic_make_wheat
#define color_generic_make_wheat

// ::color::make::wheat( c )

// TODO #include "model.hpp"
#include "../constant/wheat.hpp"


 namespace color
  {
   namespace constant
    {

     template< typename category_name >
      struct make< ::color::constant::wheat_t, category_name >
       {
        typedef category_name                         category_type;

        typedef ::color::model<category_type>            model_type;
        typedef ::color::rgb<std::uint8_t>                 rgb_type;
        typedef ::color::constant::wheat_t            constant_type;

        typedef typename ::color::trait::container<category_type>::output_type       container_output_type;

        
        inline static void process( container_output_type & container )
         {
          static model_type  s_model{ rgb_type{ 0xF5, 0xDE, 0xB3 } };
          container = s_model.container();
         }
       };

    }
  }

#endif
