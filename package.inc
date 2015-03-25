<?php
/**
 * @file
 * Migrations for the Package content type.
*/

class PackageMigration extends XMLMigration {
	public function __construct($arguments) {
    	parent::__construct($arguments);
		$this->description = t('InStyle Package Content Type Migration');
		$this->dependencies = array('PackageHub');

		$fields = array(
			'field_legacy_id' => t('Vignette component_id'), 
			'column_id' => t('Vignette ...'),
			'field_headline' => t('Vignette ...'),
			'field_category' => t('Vignette Channel'),
			'field_subcategory' => t('Vignette SubChannel'),
			'field_tag' => t('Vignette Channel'),
			'field_season_tag' => t('Vignette Channel'),
			'field_flex_promo1' => t('Vignette ...'),
			'field_flex_promo2' => t('Vignette ...'),
			'field_seo_description' => t('Vignette description'),
			'field_seo_keywords' => t('Vignette ...'),
			'field_ad_zone_override' => t('Vignette ...'),
			'field_legacy_url' => t('Vignette ...'),
			'field_creation_date' => t('Drupal Created timestamp'),
			'field_modified_date' => t('Drupal Modified timestamp'),
			'field_migrated' => t('Vignette ...'),
			'field_hub_item' => t('Vignette ...'), 
			'field_package_hubs' => t('package hubs grouping; populated by PrepareRow'), 
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
		$xml_folder = DRUPAL_ROOT . '/sites/default/files/migration/vignette/package';
		$files = file_scan_directory($xml_folder, '/\.xml$/');
		$items_url = array_keys($files);
		// drush_print_r($items_url);

		$item_xpath = '/root/package'; // relative to document
		$item_ID_xpath = 'fields/field_legacy_id'; // relative to item_xpath

		$source_options = array(
			'cache_counts' => TRUE,
		); 

		// Source and destination
		$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
		
		$this->destination = new MigrateDestinationNode('package');

		// Mapped fields
		$this->addFieldMapping('field_legacy_id', 'field_legacy_id')
			->xpath('fields/field_legacy_id');

		$this->addFieldMapping('title', 'title')
			->xpath('fields/field_headline');

		$this->addFieldMapping('field_headline', 'field_headline')
			->xpath('fields/field_headline');

		$this->addFieldMapping('field_category', 'field_category')
			->xpath('fields/field_category')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_subcategory', 'field_subcategory')
			->xpath('fields/field_subcategory')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_seasons', 'field_season_tag')
			->xpath('fields/field_season_tag')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_tags', 'field_tag')
			->xpath('fields/field_tag')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_flex_promo_1', 'field_flex_promo1')
			->xpath('fields/field_flex_promo1');

		$this->addFieldMapping('field_flex_promo_2', 'field_flex_promo2')
			->xpath('fields/field_flex_promo2');

		$this->addFieldMapping('metatag_title', 'field_seo_title')
			->xpath('fields/field_seo_title');

		$this->addFieldMapping('metatag_description', 'field_seo_description')
			->xpath('fields/field_seo_description');

		$this->addFieldMapping('metatag_keywords', 'field_seo_keywords')
			->xpath('fields/field_seo_keywords');

		$this->addFieldMapping('field_ad_zone_override', 'field_ad_zone_override')
			->xpath('fields/field_ad_zone_override');

		$this->addFieldMapping('field_legacy_url', 'field_legacy_url')
			->xpath('fields/field_legacy_url');

		$this->addFieldMapping('created', 'field_creation_date')
			->xpath('fields/field_creation_date');

		$this->addFieldMapping('changed', 'field_modified_date')
			->xpath('fields/field_modified_date');

		$this->addFieldMapping('field_package_hub', 'field_package_hubs')
			->xpath('field_package_hubs')
		    ->separator(',')
		    ->sourceMigration('PackageHub')
		    ->arguments(array('source_type' => 'nid'));

		// Unmapped source fields
			$this->addFieldMapping('column_id')
			->issueGroup(t('DNM'));	
			$this->addFieldMapping('field_migrated')
			->issueGroup(t('DNM'));	

		// Unmapped destination fields

	}

	public function prepareRow($row) {
	    // Build package hubs
		$field_hubs = array();
	    foreach ($row->xml->components->field_hub_items->field_hub_item as $field_hub) {
			$field_hubs[] = (string) $field_hub;
		}
		// drush_print_r($field_hubs);
		
		// Concat slides legacy-ids into one string
		$row->xml->field_package_hubs = implode(",", $field_hubs);
		// drush_print_r($row);

		return TRUE;
	}
}
