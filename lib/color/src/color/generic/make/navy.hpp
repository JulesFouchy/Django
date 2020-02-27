#ifndef  color_generic_make_navy
#define color_generic_make_navy

// ::color::make::navy( c )

// TODO #include "model.hpp"
#include "../constant/navy.hpp"


 namespace color
  {
   namespace constant
    {

     template< typename category_name >
      struct make< ::color::constant::navy_t, category_name >
       {
        typedef category_name                         category_type;

        typedef ::color::model<category_type>            model_type;
        typedef ::color::rgb<double>                       rgb_type;
        typedef ::color::constant::navy_t             constant_type;

        typedef typename ::color::trait::container<category_type>::output_type       container_output_type;


        inline static void process( container_output_type & container )
         {
          static model_type  s_model{ rgb_type{ 0, 0, 0.5 } };
          container = s_model.container();
         }
       };

    }
  }

#endif
