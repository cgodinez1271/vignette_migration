<?php
/**
 * @file
 * Migrations for the Video content type.
*/

class VideoMigration extends XMLMigration {
	public function __construct($arguments) {
    	parent::__construct($arguments);
		$this->description = t('InStyle Video Content Type Migration');
		$this->dependencies = array('ImageComponent');

		$fields = array(
			'field_legacy_id' => t('Vignette ...'), 
			'column_id' => t('Vignette component_id'), 
			'field_headline' => t('Vignette headline'),
			'field_description' => t('Vignette description'),
			'field_category' => t('Vignette Channel'),
			'field_subcategory' => t('Vignette SubChannel'),
			'field_images' => t('Vignette image'), 
			'field_video_playlist_id' => t('Vignette ...'), 
			'field_video_id' => t('Vignette ...'), 
			'field_player_id' => t('Vignette ...'), 
			'field_migrate' => t('Vignette ...'), 
		);

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'field_legacy_id' => array(
				'type' => 'int',
				'not null' => TRUE,
				),
			),	
			MigrateDestinationNode::getKeySchema()
		);

		// XML File
		$xml_folder = DRUPAL_ROOT . '/sites/default/files/migration/vignette/video';
		$files = file_scan_directory($xml_folder, '/\.xml$/');
    		$items_url = array_keys($files);
		// drush_print_r($items_url);

		$item_xpath = '/root/video'; // relative to document
		$item_ID_xpath = 'fields/field_legacy_id'; // relative to item_xpath

        $source_options = array(
			'cache_counts' => TRUE,
		); 		

		// Source and destination
		$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
		
		$this->destination = new MigrateDestinationNode('video');

		// Mapped fields
		$this->addFieldMapping('field_legacy_id', 'field_legacy_id')
			->xpath('fields/field_legacy_id');

		$this->addFieldMapping('title', 'title')
			->xpath('fields/field_headline');

		$this->addFieldMapping('field_headline', 'field_headline')
			->xpath('fields/field_headline');

		$this->addFieldMapping('field_description', 'field_description')
			->xpath('fields/field_description');

		$this->addFieldMapping('field_category', 'field_category')
			->xpath('fields/field_category')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_subcategory', 'field_subcategory')
			->xpath('fields/field_subcategory')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_video_thumbnail', 'field_images')
			->xpath('fields/field_images')
		    ->sourceMigration('ImageComponent')
		    ->arguments(array('source_type' => 'nid'));

		$this->addFieldMapping('field_video_playlist_id', 'video_playlist_id')
			->xpath('fields/field_video_playlist_id');

		$this->addFieldMapping('field_video_id', 'field_video_id')
			->xpath('fields/field_video_id');

		$this->addFieldMapping('field_player_id', 'field_player_id')
			->xpath('fields/field_player_id');

		// Unmapped source fields
			$this->addFieldMapping('column_id')
			->issueGroup(t('DNM'));	
			$this->addFieldMapping('field_migrated')
			->issueGroup(t('DNM'));	

		// Unmapped distnation fields

	}

}
