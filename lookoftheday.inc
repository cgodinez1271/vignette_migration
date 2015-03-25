<?php
/**
 * @file
 * Migrations for the Look of the Day content type.
*/

class LookOfTheDayMigration extends XMLMigration {
	public function __construct($arguments) {
    	parent::__construct($arguments);
		$this->description = t('InStyle LOTD Content Type Migration');
		$this->dependencies = array('ImageComponent');
		
		$fields = array(
			'field_legacy_id' => t('Vignette legay component_id'), 
			'column_id' => t('Vignette Column ID'),
			'field_gallery_type' => t('Vignette Gallery Type'), 
			'field_subcategory' => t('Vignette Gallery Category'), 
			'field_headline' => t('Vignette headline'),  
			'field_title' => t('Vignette Gallery Headline'),
			'field_category' => t('Vignette Gallery Category'), 
			'field_edit_date' => t('Vignette Edit Date'),
			'field_display_ads' => t('Vignette Display Ads Flag'),
			'field_legacy_url' => t('Vignette legacy_url'),
			'field_creation_date' => t('Drupal Created timestamp'),
			'field_modified_date' => t('Drupal Changed timestamp'),
			'field_not_searchable' => t('Vignette legacy_url'),
			'is_published' => t('Vignette published flag'),
			'field_images' => t('Vignette gallery images'), 
			'field_slides' => t('Slides Grouping variable; populated by PrepareRow'), 
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
		$xml_folder = DRUPAL_ROOT . '/sites/default/files/migration/vignette/lookoftheday';
		$files = file_scan_directory($xml_folder, '/\.xml$/');
		$items_url = array_keys($files);
		// drush_print_r($items_url);

		$item_xpath = '/root/gallery'; // relative to document
		$item_ID_xpath = 'fields/field_legacy_id'; // relative to item_xpath

        $source_options = array(
			'cache_counts' => TRUE,
		); 

		// Source and destination
		$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
		
		$this->destination = new MigrateDestinationNode('gallery');

		// Mapped fields
		$this->addFieldMapping('field_legacy_id', 'field_legacy_id')
			->xpath('fields/field_legacy_id');

		$this->addFieldMapping('field_gallery_type', 'field_gallery_type')
			->xpath('fields/field_gallery_type');

		$this->addFieldMapping('field_subcategory', 'field_subcategory')
			->xpath('fields/field_subcategory')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_headline', 'field_headline')
			->xpath('fields/field_headline');

		$this->addFieldMapping('title', 'field_title')
			->xpath('fields/field_title');

		$this->addFieldMapping('field_category', 'field_category')
			->xpath('fields/field_category')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_edit_date', 'field_edit_date')
			->xpath('fields/field_edit_date');

		$this->addFieldMapping('field_display_ads', 'field_display_ads')
			->xpath('fields/field_display_ads');

        $this->addFieldMapping('field_legacy_url', 'field_legacy_url')
			->xpath('fields/field_legacy_url');

		$this->addFieldMapping('created', 'field_creation_date')
			->xpath('fields/field_creation_date');

		$this->addFieldMapping('changed', 'field_modified_date')
			->xpath('fields/field_modified_date');

		$this->addFieldMapping('field_not_searchable', 'field_not_searchable')
			->xpath('fields/field_not_searchable');

		$this->addFieldMapping('field_slides', 'field_slides')
			->xpath('field_slides')
		    ->separator(',')
		    ->sourceMigration('ImageComponent')
		    ->arguments(array('source_type' => 'nid'));

		// Unmapped source fields
		$this->addFieldMapping('column_id')
			->issueGroup(t('DNM'));
		$this->addFieldMapping('is_published')
			->issueGroup(t('DNM'));

		// Unmapped destination fields

	}

	public function prepareRow($row) {

		// true -> 1, false -> 0
		$fda = $row->xml->fields->field_display_ads; 
		$row->xml->fields->field_display_ads = filter_var($fda, FILTER_VALIDATE_BOOLEAN);

		// No images - don't import.
		if (!isset($row->xml->components->field_images)) {
			  return FALSE;
		}

	    // Build slides
		$field_slides = array();
	    foreach ($row->xml->components->field_images->field_image as $field_image) {
			$field_slides[] = (string) $field_image;
		}

		// Concat slides legacy-ids into one string 
		$row->xml->field_slides = implode(",", $field_slides);

		// drush_print_r($row);
		return TRUE;
	}
}
