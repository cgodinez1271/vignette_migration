<?php
/**
 * @file
 * Migrations for the Image content type.
*/

class ImageFileCopyMigration extends XMLMigration {
	public function __construct($arguments) {
		parent::__construct($arguments);
		$this->description = t('Profile images');

		$fields = array(
			'field_legacy_id' => t('Vignette component_id'),
			'field_image' => t('Vignette image location'),
			'imagename' => t('prepareRow'),
		);

		$this->map = new MigrateSQLMap($this->machineName,
			array('field_legacy_id' => array(
				'type' => 'int',
				'unsigned' => TRUE,
				'not null' => TRUE,
				'description' => 'Image ID.'
				)	    
			),   
			MigrateDestinationFile::getKeySchema()
		);   

		// XML File
		$xml_folder = DRUPAL_ROOT . '/sites/default/files/migration/vignette/imagefile';
		$files = file_scan_directory($xml_folder, '/\.xml$/');
		$items_url = array_keys($files);
		// drush_print_r($items_url);

		$item_xpath = '/root/image-component'; // relative to document
		$item_ID_xpath = 'field_legacy_id'; // relative to item_xpath

		$source_options = array(
			'cache_counts' => TRUE,
	     );  
	
		$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
		$this->destination = new MigrateDestinationFile();
		
		$this->addFieldMapping('value', 'field_image')
			->xpath('field_image');
		$this->addFieldMapping('destination_file', 'imagename');	

		$this->addUnmigratedDestinations(array(
			'fid',
			'file_replace',
			'preserve_files',
			'timestamp',
			'uid',
			'urlencode',
		));  

	$this->removeFieldMapping('pathauto');
	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

		$image_path = (string) $row->xml->field_image;
		//drush_print_r($image_path);
		$file = basename($image_path);
		$file_info = pathinfo($file);

		// No file extension? No import.
		if (!isset($file_info['extension']) || !$file_info['extension']) {
			return FALSE;
		}

		$valid_image_extensions = array(
			'png',
			'gif',
			'jpg',
			'jpeg',
		);

		if (!in_array($file_info['extension'], $valid_image_extensions)) {
			return FALSE;
		}

		$count = 0;
		$row->imagename = preg_replace('/^http:\/\/img2\.timeinc\.net\/instyle\//','', $image_path, -1, $count);
		if ($count < 1) { return FALSE; }
	}
}
