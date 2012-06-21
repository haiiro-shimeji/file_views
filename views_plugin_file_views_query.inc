<?php

class views_plugin_file_views_query extends views_plugin_external_query {
	
	protected function getResults() {
		
		if ( ( $root_dir = preg_replace( '%/$%', '', variable_get('file_views_path', '') ) ) ) {
		
			$result = array();

			$dir = preg_replace( '%\.\.%', '', preg_replace( '%/$%', '', implode('/', $view->args) ) );
			$real_path = $root_dir.'/'.$dir;

			if (( $handle = opendir($real_path))) {

				while (false !== ($entry = readdir($handle))) {

					if ( preg_match( '%^\.%', $entry ) ) continue;
					
					$entry_real_path = $real_path.'/'.$entry;

					$result[] = (Object)array(
						'file_views_name' => $entry,
						'file_views_type' => ( is_dir( $entry_real_path ) ? 'd' : 'f' ),
						'file_views_path' => $dir.'/'.$entry.( is_dir( $entry_real_path ) ? '/' : ''),
						'file_views_modified' => 1000,
					);

				}

			}
			
			return $result;
			
		} else 
			
			return array();

	}

	protected function _add_field($table, $field, $alias = '', $params = array()) {
		return 'file_views_'.$field;
	}
	
	protected function compareObject( $a, $b ) {
		return ( $a->file_views_name == $b->file_views_name 
				? 0
				: ( $a->file_views_name < $b->file_views_name 
					? -1 : 1 ) 
		);
	}
	
}