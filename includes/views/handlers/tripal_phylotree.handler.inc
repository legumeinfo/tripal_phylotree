<?php
class tripal_phylotree_views_handler_field_org_count extends views_handler_field {
  function render($values) {
    $counts_by_org = $this->options['counts_by_org'];
    $tokens = $this->get_render_tokens('');
    if(!empty($tokens)) {
      $counts_by_org = $tokens[$counts_by_org];

      $by_orgs_raw = json_decode(html_entity_decode($counts_by_org));
      $by_orgs = array();
      foreach ($by_orgs_raw as $value) {
        // Using array_merge() is tempting, but it will make array keys unusable https://stackoverflow.com/a/35180513
        foreach ($value as $key => $value2) {
          $by_orgs[$key] = $value2;
        }
      }

      $org_id = substr($this->options['id'], 0, -6);

      if (isset($by_orgs[$org_id])) {
        return $by_orgs[$org_id];
      }
    }
    return 0;
  }

  function option_definition() {
    $options = parent::option_definition();
    //define our custom settings fields
    $options['counts_by_org'] = array();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $options[t('Fields')]["[$field]"] = $handler->ui_name();
    }
    $form['counts_by_org'] = array(
      '#type' => 'select',
      '#title' => t('Count by orgs'),
      '#options' => $options,
      '#default_value' => isset($this->options['counts_by_org']) ? $this->options['counts_by_org'] : '',
      '#required' => TRUE
    );
    parent::options_form($form, $form_state);
  }

  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
    //update our custom values
    $this->options['counts_by_org'] = $form_state['values']['options']['counts_by_org'];
  }
}
