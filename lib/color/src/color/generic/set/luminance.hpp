#ifndef color_generic_set_luminance
#define color_generic_set_luminance

// ::color::set::luminance( c, val )

 namespace color
  {
   namespace set
    {

     template< typename category_name >
      inline
      void
      luminance
       (
                 ::color::model< category_name >                                   & color_parameter,
        typename ::color::model< category_name >::component_input_const_type         component_parameter
       )
       {
        // Do nothing!!!
       };

    }
  }

#endif
